Ract-gearOOT=.
CC=g++

OCVLIB=-lopencv_core -lopencv_imgproc -lopencv_highgui -L/usr/lib64 -L/opt/opencv/lib/ -lopencv_features2d -lopencv_flann -lopencv_calib3d -lopencv_video -lpthread -lQtGui -lQtCore
ifeq ($(OCVENV), enseirb)
 CFLAGS  = -Wall -Wextra -Werror -I$(ROOT) -I/opt/opencv/include
 LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib $(OCVLIB)
else
 CFLAGS  = -Wall -Wextra -Werror -I$(ROOT)
 LDFLAGS = $(OCVLIB)
endif

TEST_FOLDER=Experiments_log/lot1/
RESULT_FOLDER=result/

BIN = \
	field\
	compare


all: $(BIN)
	mkdir -p $(RESULT_FOLDER)


test:
	n=1 ; while [[ $$n -le 30 ]] ; do \
		./field $(TEST_FOLDER)$${n}.png $${n}_mask; true; \
		./compare $(TEST_FOLDER)$${n}_mask.png $(RESULT_FOLDER)$${n}_mask.png; true; \
		((n = n + 1)) ; \
	done

%: %.cpp %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *~ *.png
cleanall: clean
	$(RM) $(BIN) *.o *.pdf
	$(RM) -R  $(RESULT_FOLDER)

.PHONY: all clean cleanall test-deploy extract-gear extract-gear
