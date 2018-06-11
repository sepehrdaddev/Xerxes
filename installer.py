#!/usr/bin/env python3


def run(cmd):
    process = Popen('{}'.format(cmd), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    return process.returncode


def compile_openssl():
    url = "https://www.openssl.org/source/openssl-1.1.0h.tar.gz"
    current_path = getcwd()
    script = "mv openssl-1.1.0h.tar.gz /tmp && cd /tmp && tar xvf openssl-1.1.0h.tar.gz && cd openssl-1.1.0h " \
             "&& ./config no-shared && make -j 4 && make install -j 4 " \
             "&& rm -rf /tmp/cd openssl-1.1.0h && cd {}".format(current_path)
    if run("{} {}".format("curl", url)) != 0:
        print("Dependency installation failed...")
        exit(1)
    else:
        if run(script) != 0:
            print("Dependency installation failed...")
            exit(1)


def dependency_install(silent=False):
    distro = linux_distribution()[0].lower()
    dep_script = {'debian': 'apt-get update && apt-get -y install build-essential cmake libssl-dev pkgconf curl',
                  'fedora': 'yum -y install cmake openssl-devel pkgconf gcc-c++ curl'}
    script = ''
    if distro in ('debian', 'ubuntu', 'kali', 'parrot'):
        script = dep_script['debian']
    elif distro in ('fedora', 'centos', 'rhel', 'redhat', 'red hat'):
        script = dep_script['fedora']
    if not silent:
        print("Installing Dependencies...")
    if run(script) != 0:
        print("Dependency installation failed...")
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
    if run('cmake .. && make -j 4') != 0:
        if path.isfile("/usr/lib/libcrypto.a") or path.isfile("/usr/lib64/libcrypto.a"):
            print("Compilation failed...")
            exit(1)
        else:
            compile_openssl()
            compile(silent)
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

    if path.isabs('docker'):
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
        if run('docker build . -t xerxes') != 0:
            print("Docker build failed...")
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
    from subprocess import Popen, PIPE
    from os import name, geteuid, mkdir, chdir, path, symlink, unlink, remove, getcwd
    from sys import argv, exit
    from shutil import rmtree, copy

    exit(main(argv))
