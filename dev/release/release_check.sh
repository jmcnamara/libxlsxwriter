#!/bin/bash

clear
echo "|"
echo "| Pre-release checks."
echo "|"
echo


#############################################################
#
# Run tests.
#
function check_test_status {

    echo
    echo -n "Are all tests passing?                     [y/N]: "
    read RESPONSE

    if [ "$RESPONSE" != "y" ]; then

        echo -n "    Run all tests now?                 [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please run: make test\n";
            exit 1
        else
            echo "    Running tests...";
            make test
            check_test_status
         fi
    fi
}


#############################################################
#
# Check Changes file is up to date.
#
function check_changefile {
    clear

    echo "Latest change in Changes file: "
    perl -ne '$rev++ if /^##/; exit if $rev > 1; print "    | $_"' Changes.txt

    echo
    echo -n "Is the Changes file updated?               [y/N]: "
    read RESPONSE

    if [ "$RESPONSE" != "y" ]; then
        echo
        echo -e "Please update the Change file to proceed.\n";
        exit 1
    fi
}


#############################################################
#
# Check the versions are up to date.
#
function check_versions {

    clear
    echo
    echo "Latest file versions: "

    grep -He "[0-9]\.[0-9]\.[0-9]" include/xlsxwriter.h libxlsxwriter.podspec | sed 's/:/ : /g' | awk '{printf "    | %-24s %s\n", $1, $5}'

    echo
    echo -n "Are the versions up to date?               [y/N]: "
    read RESPONSE

    if [ "$RESPONSE" != "y" ]; then
        echo -n "    Update versions?                   [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please update the versions to proceed.\n";
            exit 1
        else
            echo "    Updating versions...";
            perl -i dev/release/update_revison.pl include/xlsxwriter.h libxlsxwriter.podspec
            check_versions
         fi
    fi
}

#############################################################
#
# Check that the docs build cleanly.
#
function check_docs {

    # clear
    echo
    echo -n     "Do the docs build cleanly?             [y/N]: "
    read RESPONSE


    if [ "$RESPONSE" != "y" ]; then

        echo -n "    Build docs now?                    [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please run: make docs\n";
            exit 1
        else
            echo "    Running tests...";
            make docs
            check_docs
         fi
    fi
}


#############################################################
#
# Run release checks.
#
function check_git_status {

    clear

    echo "Git status: "
    git status | awk '{print "    | ", $0}'

    echo "Git log: "
    git log -1 | awk '{print "    | ", $0}'

    echo "Git latest tag: "
    git tag -l -n1 | tail -1 | awk '{print "    | ", $0}'

    echo
    echo -n "Is the git status okay?                    [y/N]: "
    read RESPONSE

    if [ "$RESPONSE" != "y" ]; then
        echo
        echo -e "Please fix git status.\n";

        git tag -l -n1 | tail -1 | perl -lane 'printf "git commit -m \"Prep for release %s\"\ngit tag \"%s\"\n\n", $F[4], $F[0]' | perl dev/release/update_revison.pl
        exit 1
    fi
}

check_test_status
clear
check_docs
check_changefile
check_versions
check_git_status


#############################################################
#
# All checks complete.
#
clear
echo
echo "Interface configured [OK]"
echo "Versions updated     [OK]"
echo "Git status           [OK]"
echo
echo "Everything is configured.";
echo

echo -n "Confirm release:                               [y/N]: ";
read RESPONSE

if [ "$RESPONSE" == "y" ]; then
    exit 0
else
    exit 1
fi
