#include "Rc5Decoder.h"

String Rc5Decoder::toString() const {
    return

            isValid() ?
#ifdef ARDUINO
            String(F("RC5 ")) + String(D) + F(" ") + String(F) + F(" ") + String(T)
#else
            String("RC5 ") + std::to_string(D) + String(" ") + std::to_string(F) + String(" ") + std::to_string(T)
#endif
            : String();
}

Rc5Decoder::Length Rc5Decoder::decode(microseconds_t t) {
    Length len =  (t < timebaseLower) ? invalid
            : (t <= timebaseUpper) ? half
            : (t >= 2*timebaseLower && t <= 2*timebaseUpper) ? full
            : invalid;
    return len;
}

unsigned int Rc5Decoder::decode(microseconds_t flash, microseconds_t gap) {
    boolean result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 3) ? 1
            : getDuration(gap, 1) ? 0
            : invalid;
}

boolean Rc5Decoder::tryDecode(const IrReader& irCapturer, Stream& stream) {
    Rc5Decoder decoder(irCapturer);
    return decoder.printDecode(stream);
}

Rc5Decoder::Rc5Decoder(const IrReader& irCapturer) {
    //super(irSequence);
    unsigned int index = 0U;
    unsigned int sum = 0U;
    int doublet = -1;

    while (doublet < 25) {
        Length length = decode(irCapturer.getDuration(index++));
        if (length == invalid)
            return;
        doublet += (int) length;
        if (doublet % 2 == 1)
            sum = (sum << 1U) + (index & 1U);
    }
    sum = ~sum & 0x1FFFU;

    boolean success = getEnding(irCapturer.getDuration(irCapturer.getDataLength()-1));
    if (!success)
        return;

    F = (sum & 0x3FU) | ((~sum & 0x1000U) >> 6U);
    D = (sum & 0x7C0U) >> 6U;
    T = (sum & 0x0800U) >> 11U;

    setValid(true);
}
