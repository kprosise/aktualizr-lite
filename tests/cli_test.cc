#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/format.hpp>
#include <boost/process.hpp>

#include "cli/cli.h"
#include "composeappmanager.h"
#include "docker/restorableappengine.h"
#include "liteclient.h"

#include "test_utils.h"
#include "uptane_generator/image_repo.h"

#include "fixtures/aklitetest.cc"

class CliClient : public AkliteTest {
 protected:
  std::shared_ptr<AkliteClient> createAkClient(InitialVersion initial_version = InitialVersion::kOn) {
    return std::make_shared<AkliteClient>(createLiteClient(initial_version));
  }

  TufTarget createTufTarget() {
    auto app01 = registry.addApp(fixtures::ComposeApp::create("app-01"));
    std::vector<AppEngine::App> apps{app01};
    return Target::toTufTarget(createTarget(&apps));
  }

  void reboot(std::shared_ptr<AkliteClient>& client, bool reset_bootupgrade_flag = true) {
    client.reset();
    boost::filesystem::remove(ClientTest::test_dir_.Path() / "need_reboot");
    if (reset_bootupgrade_flag) {
      boot_flag_mgr_->set("bootupgrade_available", "0");
    }
    client = std::make_shared<AkliteClient>(createLiteClient(InitialVersion::kOff, app_shortlist_, false));
  }
};

TEST_P(CliClient, FullUpdate) {
  auto akclient{createAkClient()};
  const auto target01{createTufTarget()};

  ASSERT_EQ(cli::Install(*akclient, target01.Version()),
            cli::StatusCode::
                UnknownError /*TBD: replace with the correct status code, currently just the code stub is invoked*/);
  reboot(akclient);
  ASSERT_EQ(cli::CompleteInstall(*akclient),
            cli::StatusCode::
                UnknownError /*TBD: replace with the correct status code, currently just the code stub is invoked*/);
}

INSTANTIATE_TEST_SUITE_P(MultiEngine, CliClient, ::testing::Values("RestorableAppEngine"));

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << argv[0] << " invalid arguments\n";
    return EXIT_FAILURE;
  }

  ::testing::InitGoogleTest(&argc, argv);
  logger_init();

  // options passed as args in CMakeLists.txt
  fixtures::DeviceGatewayMock::RunCmd = argv[1];
  fixtures::SysRootFS::CreateCmd = argv[2];
  return RUN_ALL_TESTS();
}
