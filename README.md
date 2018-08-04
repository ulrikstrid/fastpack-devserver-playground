# fastpack devserver playground

Playing with serving files and websockets in reason to build a devserver for fastpack.

The project is started as a clone of [hello-reason](https://github.com/esy-ocaml/hello-reason).

The fileserver part is borrowed from [@anmonteiro's graphql example](https://github.com/anmonteiro/reason-graphql-fullstack).

## Usage

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
