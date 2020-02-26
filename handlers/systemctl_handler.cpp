#include "systemctl_handler.hpp"

#include "common_tools.hpp"

using namespace std::string_literals;

namespace TabCompletion
{
//#####################################################################################################################
    struct SystemctlHandler::Constants
    {
        const std::vector <std::string> dashCommands = {
            "h", "H", "M", "t", "p", "a", "l", "r", "T", "i", "s", "q", "f", "n", "o"
        };

        const std::vector <std::string> dashDashCommands = {
            "help", "version", "system", "user", "host", "machine", "type", "state",
            "failed", "property", "all", "full", "recursive", "reverse", "job-mode",
            "show-transaction", "show-types", "value", "ignore-inhibitors", "kill-who",
            "signal", "now", "dry-run", "quite", "wait", "no-block", "no-wall",
            "no-reload", "no-legend", "no-pager", "no-ask-password", "global", "runtime",
            "force", "preset-mode", "root", "lines", "output", "firmware-setup", "boot-loader-menu",
            "boot-loader-entry", "plain"
        };

        const std::vector <std::string> otherCommands = {
            "list-units", "list-sockets", "list-timers", "start", "stop", "reload", "restart", "try-restart",
            "reload-or-restart", "try-reload-or-restart", "isolate", "kill", "is-active", "is-failed",
            "status", "show", "cat", "set-property", "help", "reset-failed", "list-dependencies",
            "list-unit-files", "enable", "disable", "reenable", "preset", "preset-all", "is-enabled",
            "mask", "unmask", "link", "revert", "add-wants", "add-requires", "edit", "get-default",
            "set-default", "list-machines", "list-jobs", "cancel", "show-environment",
            "set-environment", "unset-environment", "import-environment", "daemon-reload", "daemon-reexec",
            "is-system-running", "default", "rescue", "emergency", "halt", "poweroff", "reboot",
            "kexec", "exit", "switch-root", "suspend", "hibernate", "hybrid-sleep", "suspend-then-hibernate"
        };
    };
//#####################################################################################################################
    SystemctlHandler::SystemctlHandler(DirectoryHandler* dHandler, std::optional <std::string> const& modulePath)
        : dHandler_{dHandler}
        , modulePath_{}
    {
        if (modulePath)
            modulePath_ = modulePath.value();
        else
            modulePath_ = "/etc/systemd/system/";
    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult SystemctlHandler::tryComplete(std::vector <Token> const& tokens, bool forceAll)
    {
        if (tokens.empty())
            return {};

        if (tokens.front().token() != "systemctl")
            return CompletionResult::makeInappropriateResult();

        const Constants consts;

        DirectoryHandler moduleHandler{modulePath_, true};

        return tryCompleteBaseCommands
        (
            tokens,
            forceAll,
            moduleHandler,
            consts.dashCommands,
            consts.dashDashCommands,
            consts.otherCommands
        );
    }
//#####################################################################################################################
}
