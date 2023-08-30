#!/bin/bash

# rename to project specific values
NAME="mr"
CONST="MR"
STRUCT="Mr"
FN="mr"

# will be replaced
MR_NAME="mr"
MR_STRUCT="Mr"
MR_CONST="MR"
MR_FN="mr"

function replace() {
	echo "Replacing $1 with $2"
	find ./ -type f -name '*.c' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.h' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.md' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.lua' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.sh' -exec sed -i "s/$1/$2/g" {} \;
	find ./doc/ -type f -name '*' -exec sed -i "s/$1/$2/g" {} \;
}

replace $MR_NAME $NAME
replace $MR_CONST $CONST
replace $MR_STRUCT $STRUCT
replace $MR_FN $FN

mv "include/$MR_NAME.h" "include/$NAME.h"
mv "src/$MR_NAME.c" "src/$NAME.c"
mv "doc/$MR_NAME" "doc/$NAME" 
