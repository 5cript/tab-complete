#include "git_handler.hpp"
#include "directory_handler.hpp"

#include "common_tools.hpp"

using namespace std::string_literals;

namespace TabCompletion
{
//#####################################################################################################################
    struct GitHandler::Constants
    {
        const std::vector <std::string> dashDashCommands = {
            "version", "help", "html-path", "man-path", "info-path", "exec-path",
            "paginate", "no-pager", "no-replace-objects", "bare", "git-dir", "work-tree", "namespace"
        };

        const std::vector <std::string> dashCommands = {
            "C", "c", "p", "P", "v"
        };

        const std::vector <std::string> otherCommands = {
            "add", "am", "archive", "bisect", "branch", "bundle", "checkout", "cherry-pick", "citool", "clean"
            "clone", "commit", "describe", "diff", "fetch", "format-patch", "gc", "gitk", "grep", "gui", "init"
            "log", "merge", "mv", "notes", "pull", "push", "range-diff", "rebase", "reset", "restore", "revert",
            "rm", "shortlog", "show", "stash", "status", "submodule", "switch", "tag", "worktree", "config",
            "fast-export", "fast-import", "filter-branch", "mergetool", "pack-refs", "prune", "reflog", "remote",
            "repack", "replace", "annotate", "blame", "count-objects", "difftool", "fsck", "gitweb", "help",
            "instaweb", "merge-tree", "rerere", "show-branch", "verify-commit", "verify-tag", "whatchanged",
            "archimport", "cvsexportcommit", "cvsimport", "cvsserver", "imap-send", "p4", "quiltimport", "request-pull",
            "send-email", "svn", "apply", "checkout-index", "commit-graph", "commit-tree", "hash-object", "index-pack",
            "merge-file", "merge-index", "mktag", "mktree", "multi-pack-index", "pack-objects", "prune-packed", "read-tree",
            "symbolic-ref", "unpack-objects", "update-index", "update-ref", "write-tree", "cat-file", "cherry", "diff-files",
            "diff-index", "diff-tree", "for-each-ref", "get-tar-commit-id", "ls-files", "ls-remote", "ls-tree", "merge-base",
            "name-rev", "pack-redundant", "rev-list", "rev-parse", "show-index", "show-ref", "unpack-file", "var",
            "verify-pack", "daemon", "fetch-pack", "http-backend", "send-pack", "update-server-info", "check-attr",
            "check-ignore", "check-mailmap", "check-ref-format", "column", "credential", "credential-cache", "credential-store",
            "fmt-merge-msg", "interpret-trailers", "mailinfo", "mailsplit", "merge-one-file", "patch-id", "sh-i18n", "sh-setup",
            "stripspace", "clang-format"
        };
    };
//#####################################################################################################################
    GitHandler::GitHandler(DirectoryHandler* dHandler)
        : dHandler_{dHandler}
    {
        if (dHandler_ == nullptr)
            throw std::invalid_argument("passing a directory handler is not optional");
    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult GitHandler::tryComplete(std::vector <Token> const& tokens, bool forceAll)
    {
        if (tokens.empty())
            return {};

        if (tokens.front().token() != "git")
            return CompletionResult::makeInappropriateResult();

        const Constants consts;

        return tryCompleteBaseCommands
        (
            tokens,
            forceAll,
            *dHandler_,
            consts.dashCommands,
            consts.dashDashCommands,
            consts.otherCommands
        );
    }
//#####################################################################################################################
}
