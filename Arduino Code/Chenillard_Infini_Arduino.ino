#define Delais 1000 // Delais en seconde
#define GPIO1 D11
#define GPIO2 D10
#define GPIO3 D9
#define GPIO4 D6

#define MAX_PWM_VALUE 255
#define PWM_INCREMENT 10

int GPIO[4] = {GPIO1, GPIO2, GPIO3, GPIO4};

void Bobine(int Port, bool up);


void setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(GPIO[i], OUTPUT);
    }
}

void loop() {
    for (int i = 0; i < 4; i++) {
        Bobine(GPIO[i], true);  // Allumer la bobine
        delay(Delais);
        Bobine(GPIO[i], false); // Éteindre la bobine
        delay(50); // Ajouter une pause courte entre les bobines
    }
    for (int i = 3; i >= 0; i--) {
        Bobine(GPIO[i], true);  // Allumer la bobine
        delay(Delais);
        Bobine(GPIO[i], false); // Éteindre la bobine
        delay(50); // Ajouter une pause courte entre les bobines
    }
}


void Bobine(int Port, bool up) {
    int val = (up) ? 0 : MAX_PWM_VALUE;
    int increment = (up) ? PWM_INCREMENT : -PWM_INCREMENT;

    while ((up && val <= MAX_PWM_VALUE) || (!up && val >= 0)) {
        analogWrite(Port, val);
        val += increment;
    }

    delay(50);
}