#
# finds the minuit libs
# @author Tobias Weber <tobias.weber@tum.de>
# @license GPLv2 or GPLv3
#

find_path(Minuit2_INCLUDE_DIRS
	NAMES MnMigrad.h
	PATH_SUFFIXES root Minuit2 Minuit root/Minuit2 root/Minuit
	HINTS /usr/include/root/Minuit2 /usr/local/include/root/Minuit2 /opt/local/include/root/Minuit2
	DOC "Root/Minuit2 include directories"
)

# also include root base dir
list(APPEND Minuit2_INCLUDE_DIRS "${Minuit2_INCLUDE_DIRS}/..")


find_library(Minuit2_LIBRARIES
	NAMES Minuit2
	HINTS /usr/lib64/root /usr/lib/root /usr/lib32/root
	DOC "Minuit2 library"
)

if(Minuit2_INCLUDE_DIRS AND Minuit2_LIBRARIES)
	set(Minuit2_FOUND 1)

	message("Minuit include directories: ${Minuit2_INCLUDE_DIRS}")
	message("Minuit library: ${Minuit2_LIBRARIES}")	
else()
	set(Minuit2_FOUND 0)

	message("Error: Minuit2 could not be found!")
endif()
