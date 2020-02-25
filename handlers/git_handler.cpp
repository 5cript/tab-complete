#include "git_handler.hpp"
#include "directory_handler.hpp"

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
    GitHandler::GitHandler(std::filesystem::path baseDir)
        : baseDir_{std::move(baseDir_)}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult GitHandler::tryComplete(std::vector <Token> const& tokens, bool forceAll)
    {
        if (tokens.empty())
            return {};

        if (tokens.front().token() != "git")
            return CompletionResult::makeInappropriateResult();

        Constants consts;

        // "git "
        if (tokens.size() == 2 && tokens.back().token() == " ")
        {
            CompletionResult res;
            for (auto i = std::begin(consts.otherCommands); i != std::end(consts.otherCommands); ++i)
                res.suggestions.push_back(*i);
        }

        // "git -"
        if (tokens.size() == 3 && tokens.back().token() == "-")
        {
            CompletionResult res;
            for (auto i = std::begin(consts.dashCommands); i != std::end(consts.dashCommands); ++i)
                res.suggestions.push_back("-"s + *i);
        }

        // "git --"
        if (tokens.size() == 3 && tokens.back().token() == "--")
        {
            CompletionResult res;
            for (auto i = std::begin(consts.dashDashCommands); i != std::end(consts.dashDashCommands); ++i)
                res.suggestions.push_back("--"s + *i);
        }

        // "git ?"
        if (tokens.size() == 3 && tokens.back().type() == TokenType::IdentifierAndPunct)
        {
            CompletionResult res;
            auto checkAgainst = tokens.back().token();
            for (auto i = std::begin(consts.otherCommands); i != std::end(consts.otherCommands); ++i)
            {
                if (i->size() >= checkAgainst.size() && i->substr(0, checkAgainst.size()) == checkAgainst)
                    res.suggestions.push_back(*i);
            }
            if (checkAgainst.size() > 2 && checkAgainst.substr(0, 2) == "--")
            {
                checkAgainst = checkAgainst.substr(2, checkAgainst.size() - 2);
                for (auto i = std::begin(consts.dashDashCommands); i != std::end(consts.dashDashCommands); ++i)
                {
                    if (i->size() >= checkAgainst.size() && i->substr(0, checkAgainst.size()) == checkAgainst)
                        res.suggestions.push_back(*i);
                }
            }
            return res;
        }

        // "git c ?"
        if (tokens.size() > 3)
        {
            std::vector <Token> tokenCut;
            if (tokens.back().type() == TokenType::IdentifierAndPunct)
                tokenCut.push_back(tokens.back());

            DirectoryHandler dh{baseDir_};
            return dh.tryComplete(tokenCut, forceAll);
        }

        return {};
    }
//#####################################################################################################################
}
