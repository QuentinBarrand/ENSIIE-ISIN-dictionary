# A C-implemented dictionary

This is a project [pmylene](https://github.com/pmylene) and I made during our
2nd semester at [ENSIIE](http://www.ensiie.fr).  

The subject was to implement a dictionary based on a 26-ary tree, with each
word having references to its bases, synonyms and derivatives.
The data structure should be imported from a definitions file, and the user
should be offered a prompt allowing him to make various requests to the data structure.  

Finally, the program should not have any memory leak.

## Compilation ![Automated build by Travis CI](https://travis-ci.org/QuentinBarrand/ENSIIE-ISIN-dictionary.png)
Very simply, using make : `$ make`.  
If you want to get all the debug features and symbols, just call the `dev`
target instead : `$ make dev`.

## Usage
`$ ./dictionary [options] <definitions file>`

**Options**  
`-h` Prints help and exits.  
`-d` For each request, prints the number of dictionary cases we've been through.  
`-p` Prints the data structure.  
`-if <commands file>` Path to a commands file. See next section.

**Definitions file**  
A list of commands that should be executed before the program starts and
interactive prompt. See next section.

## Configuration files
### Definitions file (obligatory)
This argument is obligatory. It contains a list of words that should be inserted
into the dictionary, with their bases and synonyms.
See the [included example](definitions.example).

### Commands file (optional)
This argument is optional. It contains a list of commands, separated by a
newline character (`\0`). These commands are executed before the interactive
mode is started.
See the [included example](commands.example).

## Interactive mode
After all the options (and the commands included in the commands file, if
applicable) have been processed, the user is offered a command prompt.  
You can use the following commands :  
`BASE <word>` Prints all the base words for the specified word.  
`DERI <word>` Prints all the derivative words for the specified word.  
`SYNO <word>` Prints all the synonyms of the specified word.  
`INFO <regex>` Prints all the words matching the specified regular expression.  

You can then quit the application using the `q` command.

## Developper's documentation
The documentation for this project is made using Doxygen.
It is available [here](http://doc.quba.fr/ENSIIE/ISIN/dictionary/).

## Copying / License
This free software is provided under the terms of the
[GNU General Public License 2](https://www.gnu.org/licenses/gpl-2.0.html).
[A copy](LICENSE) of the license is integrated in the repo.
