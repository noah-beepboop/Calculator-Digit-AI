# ----------------------------
# Makefile Options
# ----------------------------

NAME = DigitAI
ICON = eye.png
DESCRIPTION = "neural network"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz -ffreestanding -std=c11 -DFLOAT_MATH
CXXFLAGS = -Wall -Wextra -Oz
LIBS := -lgraphx -lce -llcd -lkeypad -lfileioc -lstdlib -lmath -llibload

# ----------------------------

include $(shell cedev-config --makefile)
