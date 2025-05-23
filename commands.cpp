// commands.cpp
#ifndef COMMANDS_CPP
#define COMMANDS_CPP

extern "C" {
    #include <alpm.h>
}
#include <stdarg.h>

#include "include/mainWindow.h"
#include "include/aboutBox.h"

void MainWindow::LoadInstalledPackages(wxCommandEvent& event) {
    alpm_errno_t err;
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman", &err);
    if (!handle) {
        wxMessageBox(wxString::Format("Failed to initialize libalpm: %s", alpm_strerror(err)), "Error", wxICON_ERROR);
        return;
    }

    alpm_db_t* local_db = alpm_get_localdb(handle);
    alpm_list_t* pkgs = alpm_db_get_pkgcache(local_db);

    for (alpm_list_t* i = pkgs; i; i = i->next) {
        alpm_pkg_t* pkg = static_cast<alpm_pkg_t*>(i->data);
        pkgList->Append(wxString::Format("%s %s", alpm_pkg_get_name(pkg), alpm_pkg_get_version(pkg)));
    }

    alpm_release(handle);
}

void MainWindow::OnAbout(wxCommandEvent& event) {
    AboutBox about(this);
    about.ShowModal();
}

void MainWindow::ListUpgradeablePackages(wxCommandEvent& event) {
    alpm_errno_t err;
    alpm_handle_t *handle = alpm_initialize("/", "/var/lib/pacman", &err);
    if (!handle) {
        wxMessageBox(wxString::Format("Failed to initilize libalpm: %s", alpm_strerror(err)), "Error");
        return;
    };
    alpm_loglevel_t(ALPM_LOG_FUNCTION);
    // Set the directory for keyring. Since this program is for arch linux we
    // will assume it uses Pacman
    alpm_option_set_gpgdir(handle, "/etc/pacman.d/gnupg");

    // Since we are using arch linux we are making the database sig level
    // optional as arch linux doesnt sign the database files
    // https://wiki.archlinux.org/title/DeveloperWiki:Repo_DB_Signing

    alpm_option_set_default_siglevel(handle, ALPM_SIG_PACKAGE | ALPM_SIG_DATABASE_OPTIONAL);
    alpm_option_set_local_file_siglevel(handle, ALPM_SIG_PACKAGE_OPTIONAL | ALPM_SIG_DATABASE_OPTIONAL);

    // Register sync databases
    alpm_db_t *core = alpm_register_syncdb(handle, "core", ALPM_SIG_DATABASE_OPTIONAL);
    alpm_db_t *extra = alpm_register_syncdb(handle, "extra", ALPM_SIG_DATABASE_OPTIONAL);

    // TODO make this be using /etc/pacman.db/mirrorlist
    // to complex for me to setup now.

    alpm_list_t *coreServers = nullptr;
    coreServers = alpm_list_add(coreServers, strdup("http://mirror.rackspace.com/archlinux/core/os/x86_64"));
    alpm_db_set_servers(core, coreServers);

    alpm_list_t *extraServers = nullptr;
    extraServers = alpm_list_add(extraServers, strdup("http://mirror.rackspace.com/archlinux/extra/os/x86_64"));
    alpm_db_set_servers(extra, extraServers);

    alpm_list_t *sync_dbs = nullptr;
    sync_dbs = alpm_list_add(sync_dbs, core);
    sync_dbs = alpm_list_add(sync_dbs, extra);

    // Listing the database and mirrors. Test purposes. Comment out for release.

    for (alpm_list_t *i = sync_dbs; i; i = i->next) {
        alpm_db_t *db = (alpm_db_t *)i->data;
        printf("Database: %s\n", alpm_db_get_name(db));

        alpm_list_t *coreServers = alpm_db_get_servers(db);
        for (alpm_list_t *s = coreServers; s; s = s->next) {
            printf("  Mirror: %s\n", (char *)s->data);
        }
    }

    // Update sync databases
    // using a interger variable to track the result and alert the user

    int dbUpdateResult = alpm_db_update(handle, sync_dbs, 0);
    if (dbUpdateResult == -1) {
        wxLogError("Failed to syncronize all databases: %s", alpm_strerror(alpm_errno(handle)));
        alpm_list_free(sync_dbs);
        alpm_release(handle);
        return;
    } else if (dbUpdateResult == 0) {
        wxMessageBox("Successfully updated all databases", "Success");
    } else if (dbUpdateResult == 1) {
        wxMessageBox("All databases were up to date, nothing done", "Success");
    };
    
    // Get installed packages
    alpm_db_t *local_db = alpm_get_localdb(handle);
    const alpm_list_t *installed = alpm_db_get_pkgcache(local_db);

    // Clear the list box
    pkgList->Clear();

    alpm_list_t *keyring_pkgs = nullptr;
    alpm_list_t *other_pkgs = nullptr;

    for (const alpm_list_t *i = installed; i; i = i->next) {
        alpm_pkg_t *local_pkg = (alpm_pkg_t *)i->data;
        const char *pkgname = alpm_pkg_get_name(local_pkg);
        const char *local_ver = alpm_pkg_get_version(local_pkg);

        alpm_pkg_t *sync_pkg = nullptr;
        if (core) sync_pkg = alpm_db_get_pkg(core, pkgname);
        if (!sync_pkg && extra) sync_pkg = alpm_db_get_pkg(extra, pkgname);
        //if (!sync_pkg && community) sync_pkg = alpm_db_get_pkg(community, pkgname);

        if (sync_pkg) {
            const char *sync_ver = alpm_pkg_get_version(sync_pkg);
            if (alpm_pkg_vercmp(sync_ver, local_ver) > 0) {
                if (strcmp(pkgname, "archlinux-keyring") == 0) {
                    keyring_pkgs = alpm_list_add(keyring_pkgs, sync_pkg);
                } else {
                    other_pkgs = alpm_list_add(other_pkgs, sync_pkg);
                }
            }
        }
    }

    alpm_list_t *sorted = alpm_list_join(keyring_pkgs, other_pkgs);

    for (alpm_list_t *i = sorted; i; i = i->next) {
        alpm_pkg_t *pkg = (alpm_pkg_t *)i->data;
        wxString entry = wxString::Format("%s %s",
                            alpm_pkg_get_name(pkg),
                            alpm_pkg_get_version(pkg));
        pkgList->Append(entry);
    }

    alpm_list_free(sorted);
    for (alpm_list_t *i = coreServers; i; i = i->next) {
    free(i->data);  // Free the strdup'ed string
    }
    alpm_list_free(coreServers);
    for (alpm_list_t *i = extraServers; i; i = i->next) {
    free(i->data);  // Free the strdup'ed string
    }
    alpm_list_free(extraServers);
    alpm_list_free(sync_dbs);
    alpm_release(handle);
}

void MainWindow::UpdateAllPackages(wxCommandEvent& event) {
    alpm_errno_t err;
    alpm_handle_t *handle = alpm_initialize("/", "/var/lib/pacman", &err);
    if (!handle) {
        wxMessageBox(wxString::Format("Failed to initilize libalpm: %s", 
            alpm_strerror(err)), "Error");
    };

    // Set the directory for keyring. Since this program is for arch linux we
    // will assume it uses Pacman
    alpm_option_set_gpgdir(handle, "/etc/pacman.d/gnupg");


    // Since we are using arch linux we are making the database sig level
    // optional as arch linux doesnt sign the database files
    // https://wiki.archlinux.org/title/DeveloperWiki:Repo_DB_Signing

    alpm_option_set_default_siglevel(handle, ALPM_SIG_PACKAGE | ALPM_SIG_DATABASE_OPTIONAL);
    alpm_option_set_local_file_siglevel(handle, ALPM_SIG_PACKAGE_OPTIONAL | ALPM_SIG_DATABASE_OPTIONAL);

    // Register sync databases
    alpm_db_t *core = alpm_register_syncdb(handle, "core", ALPM_SIG_DATABASE_OPTIONAL);
    alpm_db_t *extra = alpm_register_syncdb(handle, "extra", ALPM_SIG_DATABASE_OPTIONAL);

    // TODO make this be using /etc/pacman.db/mirrorlist
    // to complex for me to setup now.

    alpm_list_t *coreServers = nullptr;
    coreServers = alpm_list_add(coreServers, strdup("http://mirror.rackspace.com/archlinux/core/os/x86_64"));
    alpm_db_set_servers(core, coreServers);

    alpm_list_t *extraServers = nullptr;
    extraServers = alpm_list_add(extraServers, strdup("http://mirror.rackspace.com/archlinux/extra/os/x86_64"));
    alpm_db_set_servers(extra, extraServers);

    alpm_list_t *sync_dbs = nullptr;
    sync_dbs = alpm_list_add(sync_dbs, core);
    sync_dbs = alpm_list_add(sync_dbs, extra);

    // Update sync databases
    // using a interger variable to track the result and alert the user

    int dbUpdateResult = alpm_db_update(handle, sync_dbs, 0);
    if (dbUpdateResult == -1) {
        wxLogError("Failed to syncronize all databases: %s", alpm_strerror(alpm_errno(handle)));
        alpm_list_free(sync_dbs);
        alpm_release(handle);
        return;
    }/*  else if (dbUpdateResult == 0) {
        wxMessageBox("Successfully updated all databases", "Success");
    } else if (dbUpdateResult == 1) {
        wxMessageBox("All databases were up to date, nothing done", "Success");
    } */;
   
    // Get installed packages
    alpm_db_t *local_db = alpm_get_localdb(handle);
    const alpm_list_t *installed = alpm_db_get_pkgcache(local_db);

    // Clear the list box
    pkgList->Clear();

    alpm_list_t *keyring_pkgs = nullptr;
    alpm_list_t *other_pkgs = nullptr;

    for (const alpm_list_t *i = installed; i; i = i->next) {
        alpm_pkg_t *local_pkg = (alpm_pkg_t *)i->data;
        const char *pkgname = alpm_pkg_get_name(local_pkg);
        const char *local_ver = alpm_pkg_get_version(local_pkg);

        alpm_pkg_t *sync_pkg = nullptr;
        if (core) sync_pkg = alpm_db_get_pkg(core, pkgname);
        if (!sync_pkg && extra) sync_pkg = alpm_db_get_pkg(extra, pkgname);
        //if (!sync_pkg && community) sync_pkg = alpm_db_get_pkg(community, pkgname);

        if (sync_pkg) {
            const char *sync_ver = alpm_pkg_get_version(sync_pkg);
            if (alpm_pkg_vercmp(sync_ver, local_ver) > 0) {
                if (strcmp(pkgname, "archlinux-keyring") == 0) {
                    keyring_pkgs = alpm_list_add(keyring_pkgs, sync_pkg);
                } else {
                    other_pkgs = alpm_list_add(other_pkgs, sync_pkg);
                }
            }
        }
    }

    alpm_list_t *sorted = alpm_list_join(keyring_pkgs, other_pkgs);

    //The upgrade packages transaction
    //The basic workflow of a transaction is to:
    //Initialize with alpm_trans_init
    //Choose which packages to add with alpm_add_pkg and alpm_remove_pkg
    //Prepare the transaction with alpm_trans_prepare
    //Commit the transaction with alpm_trans_commit
    //Release the transaction with alpm_trans_release
    alpm_list_t *data = nullptr;
    alpm_trans_init(handle, ALPM_TRANS_FLAG_NEEDED);
    for (alpm_list_t *a = sorted; a; a = a->next) {
        alpm_pkg_t *pkg = (alpm_pkg_t *)a->data;
        alpm_add_pkg(handle, pkg);
    }
    if (alpm_trans_prepare(handle, &data) == -1) {
        wxLogError("Failed to prepare transaction: %s", 
            alpm_strerror(alpm_errno(handle)));
        for (alpm_list_t *i = data; i; i = i->next) {
            alpm_depmissing_t *miss = (alpm_depmissing_t *)i->data;
            wxString reason = wxString::Format("Missing dependency: %s (needed by %s)", 
                miss->depend->name, miss->target);
            wxLogError("%s", reason);
        }
        alpm_list_free(data);
        return;
    }
    if (alpm_trans_commit(handle, &data) == -1) {
        wxLogError("Failed to commit transaction: %s", 
            alpm_strerror(alpm_errno(handle)));
        for (alpm_list_t *i = data; i; i = i->next) {
            alpm_conflict_t *conflict = (alpm_conflict_t *)i->data;
            wxString msg = wxString::Format("Conflict: %s - %s", conflict->package1,
                conflict->reason);
            wxLogError("%s", msg);
            alpm_list_free(data);
        }
    }
    alpm_trans_release(handle);

    for (alpm_list_t *i = sorted; i; i = i->next) {
        alpm_pkg_t *pkg = (alpm_pkg_t *)i->data;
        wxString entry = wxString::Format("%s %s",
                            alpm_pkg_get_name(pkg),
                            alpm_pkg_get_version(pkg));
        pkgList->Append(entry);
    }

    alpm_list_free(sorted);
    for (alpm_list_t *i = coreServers; i; i = i->next) {
    free(i->data);  // Free the strdup'ed string
    }
    alpm_list_free(coreServers);
    for (alpm_list_t *i = extraServers; i; i = i->next) {
    free(i->data);  // Free the strdup'ed string
    }
    alpm_list_free(extraServers);
    alpm_list_free(sync_dbs);
    alpm_release(handle);
}

#endif // COMMANDS_CPP
