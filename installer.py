#!/usr/bin/env python3


def dependency_install():
    distro = linux_distribution()[0].lower()
    dep_script = {'debian': 'apt-get update && apt-get -y install build-essential cmake libssl-dev pkgconf',
                  'fedora': 'yum -y install cmake openssl-devel pkgconf gcc-c++'}
    script = ''
    if distro in ('debian', 'ubuntu', 'kali', 'parrot'):
        script = dep_script['debian']
    elif distro in ('fedora', 'centos', 'rhel', 'redhat', 'red hat'):
        script = dep_script['fedora']
    print("Installing Dependencies...")
    process = Popen('{}'.format(script), shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Dependency installation failed...")
        print(output)
        exit(1)
    else:
        print("Dependencies installed...")


def compile():
    check_os()
    dependency_install()
    rmtree('build', ignore_errors=True)
    mkdir('build')
    chdir('build')
    print("Compiling...")
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
    check_os()
    print("Installing...")
    if path.isdir('build'):
        chdir('build')
        if path.isfile('Xerxes') and path.isfile('useragents'):
            uninstall(silent=True)
            mkdir('/opt/Xerxes')
            copy('Xerxes', '/opt/Xerxes')
            copy('useragents', '/opt/Xerxes')
            symlink('/opt/Xerxes/Xerxes', '/usr/bin/Xerxes')
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


def cleanup():
    print("Cleaning up...")
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


def write_file(filename, content):
    file = open(filename, 'w')
    file.write(content)
    file.close()


def docker():
    if call(['which', 'docker'], stdout=PIPE, stderr=PIPE, stdin=PIPE):
        print('Docker is not installed')
        exit(1)
    else:
        dockerfile = 'FROM ubuntu:latest\n' \
                     + 'RUN apt-get update && apt-get install -y build-essential cmake libssl-dev pkgconf\n' \
                     + 'COPY . /usr/src/Xerxes\n' \
                     + 'WORKDIR /usr/src/Xerxes\n' \
                     + 'RUN chmod +x run.sh\n' \
                     + 'RUN mkdir build && cd build && cmake .. && make\n' \
                     + 'ENTRYPOINT ["./run.sh"]\n'

        write_file('Dockerfile', dockerfile)
        print('Dockerfile generated successfully...')
        args = input('Please write Xerxes arguments for docker container\n>>>')

        run_sh = '#!/bin/bash\n' \
                 + 'build/Xerxes {}'.format(args)
        write_file('run.sh', run_sh)
        print('run.sh generated successfully...')
        print("Building docker image...")
        process = Popen('docker build . -t xerxes', shell=True, stdout=PIPE, stderr=PIPE, stdin=PIPE)
        output = process.communicate()[0]
        if process.returncode != 0:
            print("Docker build failed...")
            print(output)
            exit(1)
        else:
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


if __name__ == '__main__':
    from platform import linux_distribution
    from subprocess import Popen, PIPE, call
    from os import name, geteuid, mkdir, chdir, path, symlink, unlink, remove
    from sys import argv, exit
    from shutil import rmtree, copy
    exit(main(argv))
