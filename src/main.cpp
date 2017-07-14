#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <signal.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "vlog.h"
#include "cfg.h"
#include "input.h"
#include "agent.h"
#include "gadget.h"

static
void showBanner(void)
{
    vlogI("#############################################");
    vlogI("###         whisper demo on device        ###");
    vlogI("#############################################");
}

static
void enableCoredump(bool enable)
{
    const struct rlimit rlim = {
        enable ? RLIM_INFINITY : 0,
        enable ? RLIM_INFINITY : 0
    };

    setrlimit(RLIMIT_CORE, &rlim);
}

static
void signalHandler(int signo)
{
    vlogI("Aborting program ...");
    exit(-1);
}

int main(int argc, char **argv)
{
    showBanner();

    signal(SIGINT,  signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGHUP,  signalHandler);
    signal(SIGTERM, signalHandler);

    char buffer[2048] = {0};
    struct option options[] = {
        { "config",         required_argument,  NULL, 'c' },
        { "coredump",       no_argument,        NULL,  1  },
        { "debug",          no_argument,        NULL,  2  },
        { "help",           no_argument,        NULL, 'h' },
        { NULL,             0,                  NULL,  0  }
    };
    int opt;
    int idx;
    bool waitForDebug = false;

    while ((opt = getopt_long(argc, argv, "c:h?", options, &idx)) != -1) {
        switch (opt) {
        case 'c':
            strcpy(buffer, optarg);
            break;

        case 1:
            enableCoredump(true);
            break;

        case 2:
            waitForDebug = true;
            break;

        case 'h':
        case '?':
        default:
            printf("\nUsage: %s [-c CONFIG_FILE] [ -h|-?]\n", argv[0]);
            exit(-1);
        }
    }

    if (waitForDebug) {
        printf("Wait for debugger attaching, process id is: %d.\n", getpid());
        printf("After debugger attached, press any key to continue......\n");
        getchar();
    }

    if (!*buffer) {
        realpath(argv[0], buffer);
        strcat(buffer, ".conf");
    }

    std::shared_ptr<CConfig> cfg(new CConfig());
    if (!cfg || !cfg->load(buffer)) {
        vlogE("Load config %s error. recheck it.", buffer);
        return -1;
    }
    cfg->dump();

    std::shared_ptr<CInput> input(new CInput());
    if (!input || !input->setup()) {
        vlogE("Setup input error.");
        return -1;
    }

    std::shared_ptr<CAgent> agent(new CAgent(input));
    if (!agent) {
        vlogE("Out of memory!!!");
        return -1;
    }

    std::shared_ptr<CGadget> bulb(new CBulb(agent.get(), false));
    if (!bulb || !bulb->open()) {
        vlogE("Open bulb gadget error");
        return -1;
    }

    std::shared_ptr<CGadget> torch(new CTorch(cfg, agent.get(), false));
    if (!torch || !torch->open()) {
        vlogE("Open torch gadget error");
        return -1;
    }

    std::shared_ptr<CGadget> brightness(new CBrightness(agent.get(), 0));
    if (!brightness || !brightness->open()) {
        vlogE("Open brightness error");
        return -1;
    }

    std::shared_ptr<CGadget> ring(new CRing(agent.get(), false));
    if (!ring || !ring->open()) {
        vlogE("Open ring gadget error");
        return -1;
    }

    std::shared_ptr<CGadget> volume(new CVolume(agent.get(), 0));
    if (!volume || !volume->open()) {
        vlogE("Open volume gadget error");
        return -1;
    }

    std::shared_ptr<CGadget> camera(new CCamera(cfg, agent.get(), false));
    if (!camera || !camera->open()) {
        vlogE("Open camera gadget error");
        return -1;
    }

    agent->addGadget(bulb);
    agent->addGadget(torch);
    agent->addGadget(brightness);
    agent->addGadget(ring);
    agent->addGadget(volume);
    agent->addGadget(camera);

    if (!agent->setup(cfg)) {
        vlogE("Setup agent error");
        return -1;
    }

    agent->run();
    vlogI("whisper demo exited", argv[0]);

    return 0;
}
