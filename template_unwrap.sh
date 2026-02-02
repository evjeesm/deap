#!/bin/sh

# Script requires 'clang-format' to work!
#
# Generates a C file that contains all source code needed for using 'stb_deap4_int'.
# Compile and link your code against this generated source code with -g flag to add debug data.
# Now you can easily debug that code, thank me later.

style="{\"BasedOnStyle\":\"Microsoft\", \"SeparateDefinitionBlocks\":\"Always\"}"
preproc="cpp -E -P -C -xc -nostdinc -DDEAP4_INT_IMPLEMENTATION stb_deap4_int.h"

# Adding system dependencies to the generated source file.
# (this can be automated, but its not that much of a dependencies so adding them manually)
tmp_file=mktemp
printf "#include <stddef.h>\n" >  $tmp_file
printf "#include <stdlib.h>\n" >> $tmp_file
printf "#include <assert.h>\n" >> $tmp_file

# Use -raw option to skip formatting
if [ $1 = "-raw" ]
then
    $preproc > file.c | paste $tmp_file - > stb_deap4_int.c
else
    $preproc | clang-format --style="$style" | cat $tmp_file - > stb_deap4_int.c
fi

# clear temporary
rm $tmp_file

# Outputing generated file
cat stb_deap4_int.c
