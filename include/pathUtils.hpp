#pragma once

#include <sys/stat.h>

enum PathType {
	PATH_IS_FILE,
	PATH_IS_DIR,
	PATH_IS_OTHER,
	PATH_NOT_FOUND,
};

enum ErrorCode {
	SUCC_200 = 0,
	FORB_403,
	NOTF_404,
	SERV_500
};

struct PathInfo {
	PathType	type;
	ErrorCode	error;
	struct stat st;
};