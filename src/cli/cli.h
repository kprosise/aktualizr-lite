#ifndef AKTUALIZR_LITE_CLI_H_
#define AKTUALIZR_LITE_CLI_H_

#include <cstdlib>
#include <string>

class AkliteClient;

namespace cli {

enum class StatusCode {
  UnknownError = EXIT_FAILURE,
  Ok = EXIT_SUCCESS,
  OkNeedsRebootForBootFw = 5,
  TufMetaPullFailure = 10,
  TufTargetNotFound = 20,
  InstallationInProgress = 30,
  NoPendingInstallation = 40,
  DownloadFailure = 50,
  DownloadFailureNoSpace = 60,
  DownloadFailureVerificationFailed = 70,
  InstallAppPullFailure = 80,
  InstallNeedsRebootForBootFw = 90,
  InstallNeedsReboot = 100,
  InstallAppsNeedFinalization = 105,
  InstallRollbackOk = 110,
  InstallRollbackNeedsReboot = 120,
  InstallRollbackFailed = 130,
};

StatusCode Install(AkliteClient &client, int version = -1, const std::string &target_name = "",
                   const std::string &install_mode = "");
StatusCode CompleteInstall(AkliteClient &client);

}  // namespace cli

#endif  // AKTUALIZR_LITE_CLI_H_
