# Develop Biology
__Programming Life__

![build](https://github.com/develop-biology/lib_bio/actions/workflows/build_ebbs-cpp.yml/badge.svg)

Main documentation for this repo is provided in the [headers repo](https://github.com/develop-biology/part_headers_bio)

## Organization
This project uses [git-subrepo](https://github.com/ingydotnet/git-subrepo) to include the following repositories:

* **headers**: inc/bio

Because of this, make sure you `git subrepo push inc/bio` when updating header files. By default, this will push to the development branch, which must be manually merged into master.

### Namespaces
Biology is broken into the following namespaces:
1. common (base) - the corollary to real-life mathematics; used everywhere.
2. physical (nodes) - adds relativity, base classes, & real-life type simulacra.
3. log (logging) - logs
4. chemical (edges) - expands on physical base classes, adding bonds, reactions, and structure.
5. molecular (values) - adds symmetry and context to chemical base classes.
6. genetic (modules) - adds the ability to combine molecules & proteins into discrete packages.
7. cellular (work) - combines all lower namespaces and makes genetic information task-oriented.
8. visceral (context) - groups related cells into tissues and provides context for the work done.
9. organism (wrapper) - discretizes tissues and provides a functional, whole object.

### Extension
In order to facilitate cross-namespace type extension, some special rules should be followed in order to create consistency between domains.

When defining extendable types, primarily through the use of `physical::Perspectives`, it is customary to define the type with a capital (i.e. Type) in the main bio namespace, ignoring any other namespaces the type would otherwise belong to.
The definitions of type-returning functions or raw types should then be placed in a lowercase namespace; again, this namespace should be directly under the main bio namespace and not in whatever sub-namespace the type would otherwise belong to.
For example, States are defined in the physical folder but not in the physical namespace. They are then defined in the `bio::state` namespace.
This snippet is from "bio/chemical/States.h"
```c++
namespace bio {
namespace state { //<- lowercase "state", the namespace

State Enabled(); //<- Capitalized "State", the type (bio::State, not bio::state::State).

} //state namespace
} //bio namespace
```
So, you can say
```c++
bio::State myState = bio::state::Enabled();
```

To recap, extendable types should follow these rules:
* Type is defined in bio namespace.
* Type is UpperCamelCase.
* Corresponding namespace is defined in bio namespace.
* Corresponding namespace is lower_snake_case.

## Directories  
Directories are broken down as follows:  
**inc**: necessary header files for the bio library.  
**src**: source files and implementations of the classes used in the bio library  

## Building

This project depends on the [eons Basic Build System](https://github.com/eons-dev/bin_ebbs), which can be installed with `pip install ebbs`.  
With that said, this is just c++ code. Build it however you want.

```
mkdir build
cd build
ebbs -l cpp
```

make install and windows instructions coming soon