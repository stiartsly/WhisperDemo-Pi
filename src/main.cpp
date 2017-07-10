#include <cstdlib>
#include <memory>
#include <string>
#include "vlog.h"

#include "cfg.h"
#include "input.h"
#include "agent.h"
#include "gadget.h"

#include <iostream>
static const char *confPath = "/wmdemo.conf";

static
const char *getAppHome(void)
{
    //TODO;
    return ".";
}

static
void showBanner(void)
{
    vlogI("#############################################");
    vlogI("###         whisper demo on device        ###");
    vlogI("#############################################");
}

int main(int argc, char **argv)
{
    showBanner();

    const char *appHome;

    appHome = getAppHome();
    if (!appHome) {
        vlogE("Get home path of application error.");
        return -1;
    }

    const std::string cfgPath(std::string(appHome) + std::string(confPath));
    std::shared_ptr<CConfig> cfg(new CConfig());
    if (!cfg || !cfg->load(cfgPath)) {
        vlogE("Load config %s error. Please check it.", confPath);
        return -1;
    }

    std::shared_ptr<CInput> input(new CInput());
    if (!input || !input->setup()) {
        vlogE("Setup input error.");
        return -1;
    }

    std::shared_ptr<CAgent> agent(new CAgent(input));
    if (!agent || !agent->setup(cfg)) {
        vlogE("Setup device error. Please check config %s.", confPath);
        return -1;
    }

    agent->addGadget(std::shared_ptr<CGadget>(new CBulb(agent.get(), false)));
    agent->addGadget(std::shared_ptr<CGadget>(new CTorch(agent.get(), false)));
    agent->addGadget(std::shared_ptr<CGadget>(new CBrightness(agent.get(),0)));
    agent->addGadget(std::shared_ptr<CGadget>(new CRing(agent.get(), false)));
    agent->addGadget(std::shared_ptr<CGadget>(new CVolume(agent.get(),0)));
    agent->addGadget(std::shared_ptr<CGadget>(new CCamera(agent.get(), false)));

    agent->run();
    vlogI("whisper twinning exited", argv[0]);

    return 0;
}
