#!/usr/bin/env python


def dependency_install():
    distro = linux_distribution()[0].lower()
    dep_script = {'debian': 'apt update && apt -y install build-essential gcc g++ cmake clang llvm libssl-dev pkgconf',
                  'fedora': 'yum -y install cmake clang llvm openssl-devel pkgconf gcc-c++'}
    script = ''
    if distro in ('debian', 'ubuntu', 'kali', 'parrot'):
        script = dep_script['debian']
    elif distro in ('fedora', 'centos', 'rhel', 'redhat', 'red hat'):
        script = dep_script['fedora']

    process = Popen('{}'.format(script), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Dependency installation failed...")
        print(output)
        exit(1)
    else:
        print("Dependencies installed...")


def compile():
    dependency_install()
    rmtree('build', ignore_errors=True)
    mkdir('build')
    chdir('build')
    process = Popen('{}'.format('cmake .. && make'), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Compilation failed...")
        print(output)
        exit(1)
    else:
        print("Successfully compiled...")
        cleanup()


def install():
    if path.isdir('build'):
        chdir('build')
        if path.isfile('Xerxes') and path.isfile('useragents'):
            uninstall(silent=True)
            mkdir('/opt/Xerxes')
            copy('Xerxes', '/opt/Xerxes')
            copy('useragents', '/opt/Xerxes')
            copy('installer.py', '/opt/Xerxes')
            symlink('/opt/Xerxes/Xerxes', '/usr/bin/Xerxes')
            print("Successfully installed...")
        else:
            compile()
            install()
    else:
        compile()
        install()


def uninstall(silent=False):
    if path.islink('/usr/bin/Xerxes'):
        unlink('/usr/bin/Xerxes')
    if path.isdir('/opt/Xerxes'):
        rmtree('/opt/Xerxes', ignore_errors=True)
    if not silent:
        print("Successfully uninstalled...")


def cleanup():
    try:
        files = ['CMakeCache.txt', 'cmake_install.cmake', 'Makefile']
        for file in files:
            remove(file)
        rmtree('CMakeFiles')
        print("Successfully cleaned up...")
    except Exception as ex:
        print("Cleanup failed...")
        print('{}'.format(ex))
        exit(1)


def usage():
    msg = 'usage: ./installer.py <options>\n'
    msg += 'options: \n'
    msg += '                        install         install Xerxes\n'
    msg += '                        uninstall       uninstall Xerxes\n'
    msg += '                        compile         compile Xerxes\n'
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
            check_os()
            if args[1] == 'compile':
                compile()
            elif args[1] == 'install':
                install()
            elif args[1] == 'uninstall':
                uninstall()
            else:
                usage()

        except IndexError:
            usage()
    else:
        print('Please run this script as root')
        return 1


if __name__ == '__main__':
    from platform import linux_distribution
    from subprocess import Popen, PIPE
    from os import name, geteuid, mkdir, chdir, path, symlink, unlink, remove
    from sys import argv, exit
    from shutil import rmtree, copy
    exit(main(argv))
