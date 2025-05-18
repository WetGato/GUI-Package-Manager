// commands.cpp
#ifndef COMMANDS_CPP
#define COMMANDS_CPP

extern "C" {
    #include <alpm.h>
}


#include "include/mainWindow.h"

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

#endif // COMMANDS_CPP
