const int DOOR_PIN   = 8;   
const int MOTION_PIN = 9;   
const int TILT_PIN   = A2;  
const int ALARM_LED  = 13;
const int STATUS_LED = 12;

volatile bool doorFlag   = false;
volatile bool motionFlag = false;
volatile bool tiltFlag   = false;
volatile bool timerFlag  = false;

volatile byte lastPortBState;
volatile byte lastPortCState;

bool alarmActive = false;
bool statusBlinkState = false;

void setup()
{
  Serial.begin(9600);

  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(MOTION_PIN, INPUT_PULLUP);
  pinMode(TILT_PIN, INPUT_PULLUP);

  pinMode(ALARM_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  PCICR |= (1 << PCIE0);

  PCMSK0 |= (1 << PCINT0); 
  PCMSK0 |= (1 << PCINT1); 

  PCICR |= (1 << PCIE1);
  
  PCMSK1 |= (1 << PCINT10); 

  lastPortBState = PINB;
  lastPortCState = PINC;

  setupTimer1();

  Serial.println("Interrupt Driven Security System Initialized");
}

void loop()
{
  handleSensorEvents();
  handleTimerEvent();
  updateOutputs();
}

void handleSensorEvents()
{
  if (doorFlag)
  {
    doorFlag = false;
    Serial.println("Door sensor triggered!");
    alarmActive = true;
  }

  if (motionFlag)
  {
    motionFlag = false;
    Serial.println("Motion detected!");
    alarmActive = true;
  }

  if (tiltFlag)
  {
    tiltFlag = false;
    Serial.println("Tilt or Vibration detected!");
    alarmActive = true;
  }
}


void handleTimerEvent()
{
  if (timerFlag)
  {
    timerFlag = false;

    statusBlinkState = !statusBlinkState;
    digitalWrite(STATUS_LED, statusBlinkState);

    Serial.println("Timer heartbeat tick");
  }
}

void updateOutputs()
{
  digitalWrite(ALARM_LED, alarmActive);
}

ISR(PCINT0_vect)
{
  byte currentState = PINB;
  byte changedBits = currentState ^ lastPortBState;

  if (changedBits & (1 << PB0))
  {
    if (!(currentState & (1 << PB0)))  
    {
      doorFlag = true;
    }
  }

  if (changedBits & (1 << PB1))
  {
    if (currentState & (1 << PB1))  
    {
      motionFlag = true;
    }
  }

  lastPortBState = currentState;
}


ISR(PCINT1_vect)
{
  byte currentState = PINC;
  byte changedBits = currentState ^ lastPortCState;

  
  if (changedBits & (1 << PC2))
  {
    if (!(currentState & (1 << PC2))) 
    {
      tiltFlag = true;
    }
  }

  lastPortCState = currentState;
}

void setupTimer1()
{
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 15624;

  TCCR1B |= (1 << WGM12);                 
  TCCR1B |= (1 << CS12) | (1 << CS10);    
  TIMSK1 |= (1 << OCIE1A);               
  
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  timerFlag = true;
}