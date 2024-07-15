EXE="z124043_mmvm"
EXE_PATH="../bin/$EXE"

DIR="tests_bin"
TEMP_DIR="tests_tmp"
REF_DIR="tests_ref"


WIDTH=200

DISASM_FOLDER="disassemble"
INTERPRET_FOLDER="interpret_debug"

test()
{
	echo "Executing file $2 with $1 and saving output..."

	mkdir -p $TEMP_DIR
	./$EXE_PATH $1 $DIR/$2.c.out &> $TEMP_DIR/$2.c.out.txt

	if [[ $1 == "-m" ]]; then
		ref_path="$REF_DIR/$INTERPRET_FOLDER/r$2.c.out.txt"
	else
		ref_path="$REF_DIR/$DISASM_FOLDER/r$2.c.out.txt"
	fi

	diff -y $ref_path $TEMP_DIR/$2.c.out.txt -W $WIDTH
}

test $1 $2
