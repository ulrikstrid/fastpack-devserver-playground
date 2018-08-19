# fastpack devserver playground

This is a playground for developing a devserver for [fastpack](https://github.com/fastpack/fastpack). The goal is to get it merged in to fastpack.

## Usage

```
fpack-devserver(1)          Fpack-devserver Manual          fpack-devserver(1)



NAME
       fpack-devserver - Run a devserver with fastpack.

SYNOPSIS
       fpack-devserver [OPTION]... [ENTRY]

ARGUMENTS
       ENTRY (absent=./src/index.js)
           The main entry point to bundle.

OPTIONS
       --help[=FMT] (default=auto)
           Show this help in format FMT. The value FMT must be one of `auto',
           `pager', `groff' or `plain'. With `auto', the format is `pager` or
           `plain' whenever the TERM env var is `dumb' or undefined.

       -o DIR, --output=DIR (absent=./bundle)
           The output directory to put files.

       -p PORT, --port=PORT (absent=3000)
           The port to serve files on.

       --proxypath=PATH, --pp=PATH (absent=api)
           The path you want to proxy to the backend.

       --proxyrewrite=PATH, --pr=PATH
           Rewrite the proxy path to this.

       --pt=HOST, --proxytarget=HOST
           The adress of the backend server you want to proxy.

       -s DIR, --source=DIR (absent=./source)
           The source folder to copy files from.

       --version
           Show version information.

EXIT STATUS
       fpack-devserver exits with the following status:

       0   on success.

       124 on command line parsing errors.

       125 on unexpected internal errors (bugs).
```

## Special thanks

@anmonteiro has helped me a lot on discord and I have borrowed most of the fileserver part from his [graphql example](https://github.com/anmonteiro/reason-graphql-fullstack).

## Development

You need Esy, you can install the beta using [npm][]:

    % npm install -g esy@latest

Then you can install the project dependencies using:

    % esy install

Then build the project dependencies along with the project itself:

    % esy build

Now you can run your editor within the environment (which also includes merlin):

    % esy $EDITOR
    % esy vim

After you make some changes to source code, you can re-run project's build using:

    % esy build

And test compiled executable:

    % esy ./_build/default/bin/Main.exe
