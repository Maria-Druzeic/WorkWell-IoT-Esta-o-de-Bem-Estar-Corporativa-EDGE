// ==========================================================
// WorkWell IoT – Estação de Bem-Estar Corporativa
// Objetivo: Monitorar temperatura, umidade, luz e qualidade do ar.
// Exibir alertas no LCD, LEDs e buzzer.
// Equipe: 
// Giovanna Dias RM: 566647 
// Maria Laura Pereira Druzeic RM: 566634 
// Marianne Mukai RM: 568001
// ==========================================================

// Bibliotecas usadas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <RTClib.h>

// Pinos dos sensores
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define LDR_PIN A0
#define GAS_PIN A2

// LEDs de status
#define GREEN_LED_PIN 8
#define YELLOW_LED_PIN 9
#define RED_LED_PIN 10

// Buzzer
#define BUZZER_PIN 7  

// Limites para alertas
const float TEMP_MIN = 20.0;
const float TEMP_MAX = 26.0;
const float HUMID_MIN = 40.0;
const float HUMID_MAX = 60.0;
const int LIGHT_MIN = 300;
const int GAS_MAX = 400;

// Ícones para o LCD
#define ICON_UMID 1
#define ICON_LUZ  2
#define ICON_AR   3
#define ICON_OK   4

// Desenhos dos ícones
byte iconUmid[8] = { B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110 };
byte iconLuz[8]  = { B00100, B01110, B11111, B01110, B00100, B00000, B00000, B00000 };
byte iconAr[8]   = { B01010, B00100, B01010, B00100, B01010, B00100, B01010, B00000 };
byte iconOk[8]   = { B00000, B01010, B00000, B00000, B10001, B01110, B00000, B00000 };

// Criação dos objetos
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;

// Variáveis para leituras
float g_temperatura = 0;
float g_umidade = 0;
int g_luminosidade = 0;
int g_qualidade_ar = 0;

// Controle de status
String g_lastLoggedStatus = "Ideal";
String g_currentStatusMessage = "Iniciando...";

// Estatísticas acumuladas
long g_totalLeituras = 0;
float g_somaTemp = 0;
float g_somaUmid = 0;
long g_somaLuz = 0;
long g_somaQualidadeAr = 0;

// Contadores de níveis
int g_contadorCritico = 0;
int g_contadorAtencao = 0;
int g_contadorConfortavel = 0;


// ==========================================================
// SETUP
// ==========================================================
void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  dht.begin();

  // Inicia o RTC
  if (!rtc.begin()) {
    lcd.print("Erro no RTC!");
    while (1); // trava se não funcionar
  }

  // Ajuste do relógio (descomentar apenas uma vez)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Define pinos de saída
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Carrega os ícones no LCD
  lcd.createChar(ICON_UMID, iconUmid);
  lcd.createChar(ICON_LUZ, iconLuz);
  lcd.createChar(ICON_AR, iconAr);
  lcd.createChar(ICON_OK, iconOk);

  displayAnimatedWelcomeScreen();
  lcd.clear();
}


// ==========================================================
// LOOP PRINCIPAL
// ==========================================================
void loop() {

  readAllSensors();        // Lê todos os sensores
  updateComfortStatus();   // Verifica alertas e buzzer
  displayMonitoring();     // Mostra dados no LCD
  printDataToSerial();     // Envia dados para o Serial

  delay(2000);             // Aguarda 2 segundos
}


// ==========================================================
// FUNÇÕES
// ==========================================================

// Tela inicial animada
void displayAnimatedWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("    WorkWell IoT    ");
  lcd.setCursor(0, 1); lcd.print("Estacao de Bem-Estar");
  lcd.setCursor(0, 3); lcd.print("     Iniciando...   ");
  delay(2000);
}


// Lê temperatura, umidade, luz e gás
void readAllSensors() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Só atualiza se leitura for válida
  if (!isnan(h) && !isnan(t)) {
    g_umidade = h;
    g_temperatura = t;

    g_totalLeituras++;
    g_somaTemp += t;
    g_somaUmid += h;
  }

  // Lê luminosidade e qualidade do ar
  g_luminosidade = analogRead(LDR_PIN);
  g_qualidade_ar = analogRead(GAS_PIN);

  g_somaLuz += g_luminosidade;
  g_somaQualidadeAr += g_qualidade_ar;
}


// Liga apenas o LED correspondente ao estado
void updateLEDs(int activeLedPin) {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(activeLedPin, HIGH);
}


// Lógica de alertas + buzzer
void updateComfortStatus() {

  String msg = "";
  int statusCode = 0;

  // Alerta mais grave: qualidade do ar ruim
  if (g_qualidade_ar >= GAS_MAX) {

    msg = "ALERTA : AR RUIM";
    statusCode = 2;

    // Toca buzzer 3 vezes
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(120);
      digitalWrite(BUZZER_PIN, LOW);
      delay(120);
    }
  }

  // Alertas críticos de temperatura
  else if (g_temperatura > TEMP_MAX) {
    msg = "ALERTA: CALOR";
    statusCode = 2;
    digitalWrite(BUZZER_PIN, HIGH); delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (g_temperatura < TEMP_MIN) {
    msg = "ALERTA: FRIO";
    statusCode = 2;
    digitalWrite(BUZZER_PIN, HIGH); delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Nível de atenção (umidade ou luz)
  else if (g_umidade > HUMID_MAX) {
    msg = "ATENCAO: UMID.ALTA";
    statusCode = 1;
    digitalWrite(BUZZER_PIN, HIGH); delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (g_umidade < HUMID_MIN) {
    msg = "ATENCAO: UMID.BAIXA";
    statusCode = 1;
    digitalWrite(BUZZER_PIN, HIGH); delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (g_luminosidade <= LIGHT_MIN) {
    msg = "ATENCAO: LUZ FRACA";
    statusCode = 1;
    digitalWrite(BUZZER_PIN, HIGH); delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Tudo ideal
  else {
    msg = "Ambiente Ideal";
    statusCode = 0;
    digitalWrite(BUZZER_PIN, LOW);
  }

  g_currentStatusMessage = msg;

  // Acende o LED conforme o status
  if (statusCode == 0) updateLEDs(GREEN_LED_PIN);
  if (statusCode == 1) updateLEDs(YELLOW_LED_PIN);
  if (statusCode == 2) updateLEDs(RED_LED_PIN);

  // Só envia alerta no Serial se mudar
  if (msg != g_lastLoggedStatus) {
    DateTime now = rtc.now();
    Serial.println("--- ALERTA ---");
    Serial.println(now.toString("DD/MM/YYYY hh:mm:ss"));
    Serial.println(msg);
    Serial.println("---------------");
    g_lastLoggedStatus = msg;
  }
}


// Mostra dados no LCD
void displayMonitoring() {

  lcd.clear();
  DateTime now = rtc.now();

  // Temperatura e umidade
  lcd.setCursor(0, 0);
  lcd.print("T "); lcd.print(g_temperatura, 1); lcd.print("C");
  lcd.setCursor(11, 0); lcd.write(ICON_UMID);
  lcd.print(" "); lcd.print(g_umidade, 0); lcd.print("%");

  // Luz e ar
  lcd.setCursor(0, 1); lcd.write(ICON_LUZ);
  lcd.print(" "); lcd.print(g_luminosidade);
  lcd.setCursor(11, 1); lcd.write(ICON_AR);
  lcd.print(" "); lcd.print(g_qualidade_ar);

  // Status
  lcd.setCursor(0, 2);
  if (g_currentStatusMessage.startsWith("ALERTA") || g_currentStatusMessage.startsWith("ATENCAO"))
    lcd.print("!");
  else
    lcd.write(ICON_OK);

  lcd.print(" " + g_currentStatusMessage);

  // Hora e data
  lcd.setCursor(0, 3);
  lcd.print(now.toString("hh:mm:ss"));
  lcd.setCursor(10, 3);
  lcd.print(now.toString("DD/MM/YYYY"));
}


// Envia dados no Monitor Serial
void printDataToSerial() {
  Serial.print("Temp: "); Serial.print(g_temperatura);
  Serial.print(" | Umid: "); Serial.print(g_umidade);
  Serial.print(" | Luz: "); Serial.print(g_luminosidade);
  Serial.print(" | Ar: "); Serial.print(g_qualidade_ar);
  Serial.print(" | Status: "); Serial.println(g_currentStatusMessage);
}
