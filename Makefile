Ract-gearOOT=.
CC=g++

OCVLIB=-lopencv_core -lopencv_imgproc -lopencv_highgui
ifeq ($(OCVENV), enseirb)
 CFLAGS  = -Wall -Wextra -Werror -I$(ROOT) -I/opt/opencv/include
 LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib $(OCVLIB)
else
 CFLAGS  = -Wall -Wextra -Werror -I$(ROOT)
 LDFLAGS = $(OCVLIB)
endif

RESULT_FOLDER=result

BIN = \
	field\
	compare

all: $(BIN)
	mkdir -p $(RESULT_FOLDER)


test-deploy: all
	@./make-se ; true
	@./dilation ; true
	@./labeling ; true
	make extract-gear
	make extract-cell


%: %.cpp %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *~ *.png
cleanall: clean
	$(RM) $(BIN) *.o *.pdf
	$(RM) -R  $(RESULT_FOLDER)

.PHONY: all clean cleanall test-deploy extract-gear extract-gear
