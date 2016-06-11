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
    echo -n     "Are all tests passing?                 [y/N]: "
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
    echo -n     "Are the versions up to date?           [y/N]: "
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
            echo "    Building docs...";
            make docs
            check_docs
         fi
    fi
}


#############################################################
#
# Generate the cocoapods umbrella file.
#
function gen_umbrella_file {

    echo
    echo -n     "Is the umbrella file up to date?       [y/N]: "
    read RESPONSE


    if [ "$RESPONSE" != "y" ]; then

        echo -n "    Update umbrella file now?          [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please update cocoapods/libxlsxwriter-umbrella.h\n";
            exit 1
        else
            echo "    Updating file...";
            perl dev/release/gen_umbrella_file.pl > cocoapods/libxlsxwriter-umbrella.h
         fi
    fi
}


#############################################################
#
# Check the cocoapods spec file.
#
function check_pod_spec {

    echo
    echo -n     "Is the coacoapod file ok?              [y/N]: "
    read RESPONSE


    if [ "$RESPONSE" != "y" ]; then

        echo -n "    Run lint now?                      [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please run: pod spec lint libxlsxwriter.podspec\n";
            exit 1
        else
            echo "    Running lint...";
            pod spec lint libxlsxwriter.podspec --use-libraries
            check_pod_spec
         fi
    fi
}



#############################################################
#
# Update the pod repo. This can take some time.
#
function update_pod_repo {

    echo
    echo -n     "Is the pod repo updated?               [y/N]: "
    read RESPONSE


    if [ "$RESPONSE" != "y" ]; then

        echo -n "    Update now?                        [y/N]: "
        read RESPONSE

        if [ "$RESPONSE" != "y" ]; then
            echo
            echo -e "Please run: pod spec lint libxlsxwriter.podspec\n";
            exit 1
        else
            echo "    Running update...";
            cd ~/.cocoapods/repos/master
            git pull --ff-only
            cd -
            update_pod_repo
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

        echo -e "\ngit add -u";
        git tag -l -n1 | tail -1 | perl -lane 'printf "git commit -m \"Prep for release %s\"\ngit tag \"%s\"\n\n", $F[4], $F[0]' | perl dev/release/update_revison.pl
        exit 1
    fi
}

check_test_status
clear
check_docs
check_changefile
clear
gen_umbrella_file
check_pod_spec
clear
update_pod_repo
check_versions
check_git_status


#############################################################
#
# All checks complete.
#
clear
echo
echo "Everything is configured.";
echo

echo -n         "Confirm release:                       [y/N]: ";
read RESPONSE

if [ "$RESPONSE" == "y" ]; then
    exit 0
else
    exit 1
fi
