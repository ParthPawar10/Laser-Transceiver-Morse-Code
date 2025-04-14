const int UNIT_LENGTH = 100;
const int BUFFER_SIZE = 5;
enum class Signal: byte {
NOISE = 0,
DIT = 1,
DAH = 2,
ELEMENTGAP = 3,
GAP = 4,
LONGGAP = 5
};
Using led because
there was no laser
module in thinker cad
and fritzing.
struct MorseCodeElement {
Signal m_signal;
unsigned long m_duration;
};
class MorseCodeBuffer {
int m_size;
int m_head;
int m_tail;
MorseCodeElement* m_buffer;
public:
MorseCodeBuffer(int size) {
size++;
m_size = size;
m_head = 0;
m_tail = 0;
m_buffer = new MorseCodeElement[size];
}
bool Enqueue(MorseCodeElement element) {
int new_tail = (m_tail + 1) % m_size;
if (new_tail == m_head) {
return false;
}
m_tail = new_tail;
m_buffer[m_tail] = element;
return true;
}
bool TryDequeue(MorseCodeElement* element) {
if (m_head == m_tail) {
return false;
}
*element = m_buffer[m_head];
m_head = (m_head + 1) % m_size;
return true;
}
int GetCount() {
if (m_head == m_tail) {
return 0;
}
return (m_tail - m_head + m_size) % m_size;
}
};
class AdaptiveLogicLevelProcessor {
int m_sensorMinValue = 1023;
int m_sensorMaxValue = 0;
int m_sensorMedianValue = 511;
unsigned long m_sensorCalibrationTime = 0;
bool m_calibrated;
public:
AdaptiveLogicLevelProcessor() {
m_sensorMinValue = 1023;
m_sensorMaxValue = 0;
m_sensorMedianValue = 511;
m_sensorCalibrationTime = 0;
}
bool process(int sensorValue, int* digitalInputValue) {
unsigned long currentTime = millis();
if (currentTime - m_sensorCalibrationTime > 5000) {
if (m_sensorMinValue < m_sensorMaxValue) {
if (m_sensorMaxValue - m_sensorMinValue > 20) {
m_sensorMedianValue = m_sensorMinValue + (m_sensorMaxValue -
m_sensorMinValue) / 2;
m_calibrated = true;
} else {
m_calibrated = false;
}
}
m_sensorMaxValue = 0;
m_sensorMinValue = 1023;
m_sensorCalibrationTime = currentTime;
}
if (m_sensorMinValue > sensorValue) {
m_sensorMinValue = sensorValue;
}
if (m_sensorMaxValue < sensorValue) {
m_sensorMaxValue = sensorValue;
}
if (!m_calibrated) {
return false;
}
*digitalInputValue = sensorValue > m_sensorMedianValue ? HIGH : LOW;
return true;
}
};
class MorseCodeElementProcessor {
unsigned long m_previousTime = 0;
int m_previousSignal = LOW;
int m_oneUnitMinValue;
int m_oneUnitMaxValue;
int m_threeUnitMinValue;
int m_threeUnitMaxValue;
int m_sevenUnitMinValue;
int m_sevenUnitMaxValue;
public:
MorseCodeElementProcessor(int unitLengthInMilliseconds) {
m_oneUnitMinValue = (int)(unitLengthInMilliseconds * 0.5);
m_oneUnitMaxValue = (int)(unitLengthInMilliseconds * 1.5);
m_threeUnitMinValue = (int)(unitLengthInMilliseconds * 2.0);
m_threeUnitMaxValue = (int)(unitLengthInMilliseconds * 4.0);
m_sevenUnitMinValue = (int)(unitLengthInMilliseconds * 5.0);
m_sevenUnitMaxValue = (int)(unitLengthInMilliseconds * 8.0);
}
bool process(int newSignal, MorseCodeElement* element) {
unsigned long currentTime = millis();
unsigned long elapsed;
bool shouldBuffer = false;
element->m_signal = Signal::NOISE;
if (m_previousSignal == LOW && newSignal == HIGH) {
elapsed = currentTime - m_previousTime;
element->m_duration = elapsed;
if (m_sevenUnitMinValue <= elapsed) {
element->m_signal = Signal::LONGGAP;
shouldBuffer = true;
} else if (m_threeUnitMinValue <= elapsed && elapsed <=
m_threeUnitMaxValue) {
element->m_signal = Signal::GAP;
shouldBuffer = true;
} else if (m_oneUnitMinValue <= elapsed && elapsed <= m_oneUnitMaxValue)
{
element->m_signal = Signal::ELEMENTGAP;
shouldBuffer = true;
} else {
element->m_signal = Signal::NOISE;
shouldBuffer = true;
}
m_previousSignal = HIGH;
m_previousTime = currentTime;
} else if (m_previousSignal == HIGH && newSignal == LOW) {
elapsed = currentTime - m_previousTime;
element->m_duration = elapsed;
if (m_threeUnitMinValue <= elapsed && elapsed <= m_threeUnitMaxValue) {
element->m_signal = Signal::DAH;
shouldBuffer = true;
} else if (m_oneUnitMinValue <= elapsed && elapsed <= m_oneUnitMaxValue)
{
element->m_signal = Signal::DIT;
shouldBuffer = true;
} else {
element->m_signal = Signal::NOISE;
shouldBuffer = true;
}
m_previousSignal = LOW;
m_previousTime = currentTime;
}
return shouldBuffer;
}
};
class MorseCodeProcessor {
private:
static const int TREE_SIZE = 255;
static constexpr char tree[TREE_SIZE] = {
'\0', '\0', '\0', '5', '\0', '\0', '\0', 'H', '\0', '\0', '\0', '4',
'\0', '\0', '\0', 'S',
'\0', '\0', '$', '\0', '\0', '\0', '\0', 'V', '\0', '\0', '\0', '3',
'\0', '\0', '\0', 'I',
'\0', '\0', '\0', '\0', '\0', '\0', '\0', 'F', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'U',
'\0', '?', '\0', '\0', '\0', '_', '\0', '\0', '\0', '\0', '\0', '2',
'\0', '\0', '\0', 'E',
'\0', '\0', '\0', '&', '\0', '\0', '\0', 'L', '\0', '"', '\0', '\0',
'\0', '\0', '\0', 'R',
'\0', '\0', '\0', '+', '\0', '.', '\0', '\0', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'A',
'\0', '\0', '\0', '\0', '\0', '\0', '\0', 'P', '\0', '@', '\0', '\0',
'\0', '\0', '\0', 'W',
'\0', '\0', '\0', '\0', '\0', '\0', '\0', 'J', '\0', '\'', '\0', '1',
'\0', '\0', '\0', '\0',
'\0', '\0', '\0', '6', '\0', '-', '\0', 'B', '\0', '\0', '\0', '=',
'\0', '\0', '\0', 'D',
'\0', '\0', '\0', '/', '\0', '\0', '\0', 'X', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'N',
'\0', '\0', '\0', '\0', '\0', '\0', '\0', 'C', '\0', ';', '\0', '\0',
'\0', '!', '\0', 'K',
'\0', '\0', '\0', '(', '\0', ')', '\0', 'Y', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'T',
'\0', '\0', '\0', '7', '\0', '\0', '\0', 'Z', '\0', '\0', '\0',
'\0', '\0', ',', '\0', 'G',
'\0', '\0', '\0', '\0', '\0', '\0', '\0', 'Q', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'M',
'\0', ':', '\0', '8', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
'\0', '\0', '\0', 'O',
'\0', '\0', '\0', '9', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '0',
'\0', '\0', '\0'
};
bool m_error;
int m_start;
int m_end;
int m_index;
Signal m_previousInput;
void reset() {
m_error = false;
m_start = 0;
m_end = TREE_SIZE;
m_index = (m_end - m_start) / 2;
}
public:
MorseCodeProcessor() {
reset();
m_previousInput = Signal::NOISE;
}
bool process(Signal input, char* output) {
bool completed = false;
if (!m_error && input == Signal::DIT) {
if (m_start == m_index) {
m_error = true;
} else {
m_end = m_index;
m_index = m_start + (m_end - m_start) / 2;
}
} else if (!m_error && input == Signal::DAH) {
if (m_end == m_index) {
m_error = true;
} else {
m_start = m_index + 1;
m_index = m_start + (m_end - m_start) / 2;
}
} else if (input == Signal::GAP || input == Signal::LONGGAP) {
completed = !m_error && tree[m_index] != 0;
if (completed) {
output[0] = tree[m_index];
output[1] = '\0';
if (input == Signal::LONGGAP) {
output[1] = ' ';
output[2] = '\0';
}
}
reset();
}
m_previousInput = input;
return completed;
}
};
constexpr char MorseCodeProcessor::tree[];
MorseCodeBuffer buffer(BUFFER_SIZE);
MorseCodeProcessor morseCodeProcessor;
AdaptiveLogicLevelProcessor logicLevelProcessor;
MorseCodeElementProcessor morseCodeElementProcessor(UNIT_LENGTH);
SIGNAL(TIMER0_COMPA_vect) {
cli();
int digitalInputValue;
if (logicLevelProcessor.process(analogRead(A0), &digitalInputValue)) {
MorseCodeElement element;
if (morseCodeElementProcessor.process(digitalInputValue, &element)) {
buffer.Enqueue(element);
}
}
sei();
}
void setup() {
Serial.begin(9600);
cli();
OCR0A = 0xAF;
TIMSK0 |= _BV(OCIE0A);
sei();
}
bool TryDequeueSafe(MorseCodeElement* element) {
cli();
bool result = buffer.TryDequeue(element);
sei();
return result;
}
char* output = new char[3];
void loop() {
MorseCodeElement element;
while (TryDequeueSafe(&element)) {
if (element.m_signal == Signal::DIT) {
Serial.print(".");
} else if (element.m_signal == Signal::DAH) {
Serial.print("-");
}
if (morseCodeProcessor.process(element.m_signal, output)) {
Serial.print('(');
Serial.print(output);
Serial.print(')');
}
if (element.m_signal == Signal::LONGGAP) {
Serial.println();
}
}
}
