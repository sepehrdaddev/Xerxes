#!/usr/bin/env python3


def dependency_install(silent=False):
    distro = linux_distribution()[0].lower()
    dep_script = {'debian': 'apt-get update && apt-get -y install build-essential cmake libssl-dev pkgconf',
                  'fedora': 'yum -y install cmake openssl-devel pkgconf gcc-c++'}
    script = ''
    if distro in ('debian', 'ubuntu', 'kali', 'parrot'):
        script = dep_script['debian']
    elif distro in ('fedora', 'centos', 'rhel', 'redhat', 'red hat'):
        script = dep_script['fedora']
    if not silent:
        print("Installing Dependencies...")
    process = Popen('{}'.format(script), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Dependency installation failed...")
        print('{}'.format(output))
        exit(1)
    else:
        if not silent:
            print("Dependencies installed...")


def compile(silent=False):
    check_os()
    dependency_install()
    rmtree('build', ignore_errors=True)
    mkdir('build')
    chdir('build')
    if not silent:
        print("Compiling...")
    process = Popen('{}'.format('cmake .. && make'), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Compilation failed...")
        print('{}'.format(output))
        exit(1)
    else:
        if not silent:
            print("Successfully compiled...")
        cleanup()


def install(silent=False):
    check_os()
    if not silent:
        print("Installing...")
    if path.isdir('build'):
        chdir('build')
        if path.isfile('Xerxes') and path.isfile('useragents'):
            uninstall(silent=True)
            mkdir('/opt/Xerxes')
            copy('Xerxes', '/opt/Xerxes')
            copy('useragents', '/opt/Xerxes')
            symlink('/opt/Xerxes/Xerxes', '/usr/bin/Xerxes')
            if not silent:
                print("Successfully installed...")
        else:
            compile()
            install()
    else:
        compile()
        install()


def uninstall(silent=False):
    check_os()
    if not silent:
        print("Uninstalling ...")
    if path.islink('/usr/bin/Xerxes'):
        unlink('/usr/bin/Xerxes')
    if path.isdir('/opt/Xerxes'):
        rmtree('/opt/Xerxes', ignore_errors=True)
    if not silent:
        print("Successfully uninstalled...")


def cleanup(silent=False):
    if not silent:
        print("Cleaning up...")
    try:
        files = ['CMakeCache.txt', 'cmake_install.cmake', 'Makefile']
        for f in files:
            remove(f)
        rmtree('CMakeFiles')
        if not silent:
            print("Successfully cleaned up...")
    except Exception as ex:
        print("Cleanup failed...")
        print('{}'.format(ex))
        exit(1)


def write_file(filename, content):
    f = open(filename, 'w')
    f.write(content)
    f.close()


def docker(silent=False):
    cmd = ''
    if name in ('nt', 'dos'):
        cmd = 'where'
    elif name in ('linux', 'posix', 'darwin', 'osx'):
        cmd = 'which'

    if call([cmd, 'docker'], stdout=PIPE, stderr=PIPE, stdin=PIPE):
        print('Docker is not installed')
        exit(1)
    else:
        compile()
        dockerfile = 'FROM alpine:latest\nCOPY ./Xerxes /opt/Xerxes\nCOPY ./useragents /opt/Xerxes/WORKDIR ' \
                     '/opt/Xerxes\nENTRYPOINT [\"/bin/sh\"]\n '
        write_file('Dockerfile', dockerfile)
        if not silent:
            print('Dockerfile generated successfully...')
            print("Building docker image...")
        process = Popen('docker build . -t xerxes', shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
        output = process.communicate()[0]
        if process.returncode != 0:
            print("Docker build failed...")
            print('{}'.format(output))
            exit(1)
        else:
            if not silent:
                print("Successfully built docker image...")
            print('for running the container execute: docker run -it xerxes')


def usage():
    msg = 'usage: ./installer.py <options>\n'
    msg += 'options: \n'
    msg += '                        install         install Xerxes\n'
    msg += '                        uninstall       uninstall Xerxes\n'
    msg += '                        compile         compile Xerxes\n'
    msg += '                        docker          make docker image from Xerxes\n'
    print(msg)


def check_os():
    if name in ('nt', 'dos'):
        print("Xerxes doesn't support Windows")
        exit(1)
    elif name in ('darwin', 'osx'):
        print("Xerxes doesn't support Mac osx")
        exit(1)
    elif name in ('linux', 'posix'):
        return


def main(args):
    if not geteuid():
        try:
            if args[1] == 'compile':
                compile()
            elif args[1] == 'install':
                install()
            elif args[1] == 'uninstall':
                uninstall()
            elif args[1] == 'docker':
                docker()
            else:
                usage()

        except IndexError:
            usage()
    else:
        print('Please run this script as root')
        return 1
    return 0


if __name__ == '__main__':
    from platform import linux_distribution
    from subprocess import Popen, PIPE, call
    from os import name, geteuid, mkdir, chdir, path, symlink, unlink, remove
    from sys import argv, exit
    from shutil import rmtree, copy

    exit(main(argv))
