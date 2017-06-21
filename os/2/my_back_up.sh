#!/bin/bash

# include hidden files preceded by a .
shopt -s dotglob

if (( $# != 1 ))
then
	echo "USAGE:" $0 "<backUpFolder>" >&2
	exit 1
fi

TARGET=$1

if [[ ! -e $TARGET ]]
then
	mkdir -p $TARGET || exit $?
elif [[ ! -d $TARGET ]]
then
	echo "ERROR:" $TARGET "is not a directory" >&2
	exit 2
fi

# function to recursively walk through all directories
function backup {
	DIR=$1
	for F in "$DIR"/*
	do
		# skip target folder if it is in the same directory
		if [[ $F -ef $TARGET ]]
		then
			continue
		fi

		# directory?
		if [[ -d $F ]]
		then
			# just copy directory recursively if the target does not exist
			if [[ ! -d $TARGET/$F ]]
			then
				echo cp -r "$F" "$TARGET/$F"
				cp -r "$F" "$TARGET/$F" || exit $?
			else
				# otherwise recurse into directory
				backup "$F"
			fi

		# updated/new file?
		elif [[ $F -nt $TARGET/$F ]] || [[ ! -e $TARGET/$F ]]
		then
			# copy to backup
			echo cp "$F" "$TARGET/$F"
			cp "$F" "$TARGET/$F" || exit $?
		fi
	done
}

backup .
