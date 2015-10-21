#ifndef IRRECEIVERPOLL_H
#define	IRRECEIVERPOLL_H

#include "IrReceiver.h"

class IrReceiverPoll : public IrReceiver {
    private:
        milliseconds_t beginningTimeout;
        milliseconds_t endingTimeout;

           /** Data buffer */
    microseconds_t *durationData; // previously rawbuf;

    /** Number of entries in durationData */
    unsigned int dataLength;

public:
    IrReceiverPoll(unsigned int captureLength = defaultCaptureLength,
            pin_t pin = defaultPin,
            boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    ~IrReceiverPoll();

    boolean isReady() const {
        return true;
    }

    void reset();

    unsigned int getDataLength() const {
        return dataLength;
    }

    microseconds_t getDuration(unsigned int i) const {
        return durationData[i];
    }

    void setEndingTimeout(milliseconds_t timeOut) {
        endingTimeout = timeOut;
    }

    milliseconds_t getEndingTimeout() const {
        return endingTimeout;
    }

    milliseconds_t getBeginningTimeout() const {
        return beginningTimeout;
    }

    void setBeginningTimeout(milliseconds_t timeOut) {
        beginningTimeout = timeOut;
    }

    /** Does the collection of the data, i.e. does not return before either
     * the signal is captured, or timeout.
     */
    void enable();

    void disable() {};

    private:

    boolean searchForStart();

    void collectData();

    void recordDuration(unsigned long t);
};

#endif	/* IRRECEIVERPOLL_H */
