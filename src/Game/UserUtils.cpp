#include "pch.h"
#include "UserUtils.h"

int UserUtils::myClamp(int v, int lo, int hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}
