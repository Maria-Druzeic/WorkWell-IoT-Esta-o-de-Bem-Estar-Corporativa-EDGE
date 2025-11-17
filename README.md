# 🏢 WorkWell IoT – Estação de Bem-Estar Corporativa

### **Monitoramento Ambiental Inteligente com LCD, RTC, Alertas e Dashboard BI**

## 👥 **Equipe**

| Nome                            | RM     |
| ------------------------------- | ------ |
| **Giovanna Dias**               | 566647 |
| **Maria Laura Pereira Druzeic** | 566634 |
| **Marianne Mukai**              | 568001 |

---

## 📌 **Descrição do Projeto**

O **WorkWell IoT** é uma estação inteligente de monitoramento ambiental voltada para ambientes corporativos, capaz de medir e exibir:

* Temperatura
* Umidade
* Luminosidade
* Qualidade do ar (sensor de gás)
* Data e hora (RTC real-time)
* Status de conforto (Ideal, Atenção ou Alerta)

O sistema utiliza **alertas visuais (LEDs)** e **sonoros (buzzer)** para avisar de forma imediata quando algum índice ambiental representa risco ou desconforto aos colaboradores.

Além disso, os dados registrados podem ser utilizados posteriormente para **análises avançadas em dashboards como Power BI**, permitindo acompanhamento histórico, insights e tomada de decisão.

---

# 🔧 Como Funciona a IoT do Projeto

A IoT funciona por meio de um fluxo contínuo de coleta, processamento e exibição de dados:

### **1️⃣ Coleta de dados**

A cada ciclo, o Arduino lê dados dos sensores:

* Temperatura e umidade → **DHT22**
* Luminosidade → **LDR**
* Qualidade do ar → **MQ GAS**
* Data e hora → **RTC DS1307/DS3231**

### **2️⃣ Processamento**

O código:

* Analisa se os valores estão dentro, acima ou abaixo dos limites ideais
* Classifica o status em:

  * **Ideal**
  * **Atenção**
  * **Alerta crítico**
* Gera respostas visuais/sonoras adequadas

### **3️⃣ Comunicação e exibição**

Os dados são exibidos no LCD 20x4:

* Linha 1: Temperatura e umidade
* Linha 2: Luz e qualidade do ar
* Linha 3: Status atual (inclui ícone ou exclamação)
* Linha 4: Data e hora

Além disso, registros de alertas são enviados ao **Monitor Serial**.

### **4️⃣ Alertas**

O sistema emite três formas de alerta:

* **LED Verde** → tudo normal
* **LED Amarelo** → alguma variável está deslocada (atenção)
* **LED Vermelho** → nível crítico

E som adicional via **buzzer**:

* Pulsos curtos → atenção
* Pulsos médios → frio ou calor
* Três toques rápidos → ar ruim

### **5️⃣ Dados para análises avançadas**

O projeto coleta diversos dados ao longo do tempo, que podem ser direcionados a dashboards como o **Power BI**.

Isso permite visualizar:

* Tendências
* Horários de pico
* Comparação entre dias
* Frequência de alertas
* Média de qualidade do ar
* Identificação de padrões ambientais nocivos

---

# 📡 Tecnologias Utilizadas

* **Arduino Uno**
* **C/C++ (Arduino IDE)**
* **Sensores físicos: DHT22, MQ, LDR**
* **LCD 20x4 I2C**
* **RTC DS1307/DS3231**
* **Wokwi (Simulação)**
* **Dashboards BI (Power BI)**
* **Protocolos: I2C, leitura analógica/digital**

---

# 🔌 Simulação no Wokwi – Arquitetura do Circuito

A implementação deste projeto foi desenvolvida e validada usando o **Wokwi**, permitindo simular todo o ecossistema IoT antes da montagem física.
A simulação inclui todos os sensores, atuadores e módulos necessários para o monitoramento ambiental corporativo.

## 🧩 Componentes Utilizados na Simulação

A simulação no Wokwi utiliza os seguintes módulos:

* **Arduino Uno**
* **Display LCD 20x4 com módulo I2C**
* **Sensor DHT22 (Temperatura e Umidade)**
* **Sensor LDR (Luminosidade)**
* **Sensor de Gás MQ (simulado)**
* **Módulo RTC DS1307/DS3231**
* **Três LEDs indicadores (Verde, Amarelo, Vermelho)**
* **Buzzer ativo**
* **Resistores de 220Ω**
* **Protoboard (meia)**
* **link da simulação: https://wokwi.com/projects/447550039404938241**
* **IMAGEM DA SIMULAÇÃO NO WOKWI**
* **<img width="1919" height="883" alt="simulação arduino" src="https://github.com/user-attachments/assets/81573b48-cd09-4131-b176-dc58a81f83f3" />**

---

## 🖥️ Estrutura e Funcionamento da Simulação

### 1️⃣ **Arduino Uno como unidade principal**

O Arduino executa o código responsável pela leitura dos sensores, controle do LCD, geração de alertas e estatísticas ambientais.

### 2️⃣ **Display LCD 20x4 via I2C**

* Conectado aos pinos **A4 (SDA)** e **A5 (SCL)** do Arduino.
* Exibe temperatura, umidade, luminosidade, qualidade do ar, horário e status atual.

### 3️⃣ **Sensores**

**✓ DHT22**

* Mede temperatura e umidade
* Ligado ao pino digital **2**

**✓ LDR**

* Entrada analógica **A0**
* Forma divisor resistivo para leitura precisa da luminosidade

**✓ Sensor de gás MQ**

* Ligado ao pino analógico **A2**
* Usado para detectar qualidade do ar e gerar alertas

**✓ RTC DS1307 / DS3231**

* Mantém data e hora
* Comunicação via I2C (SDA/SCL)

### 4️⃣ **Indicadores visuais**

* **LED Verde** → ambiente ideal
* **LED Amarelo** → estado de atenção
* **LED Vermelho** → alerta crítico

Todos controlados pelos pinos **8, 9 e 10** do Arduino.

### 5️⃣ **Buzzer**

* Ligado ao pino **7**
* Emite alertas diferentes conforme o nível de risco

  * Pulsos curtos → atenção
  * Pulsos médios → calor ou frio
  * Sequência tripla → ar ruim (gás alto)

---

## 🔗 Conexões Principais (Resumo Técnico)

| Componente   | Pino Arduino | Função              |
| ------------ | ------------ | ------------------- |
| DHT22        | 2            | Temperatura/Umidade |
| LDR          | A0           | Luminosidade        |
| MQ Gas       | A2           | Qualidade do ar     |
| LCD SDA      | A4           | Comunicação I2C     |
| LCD SCL      | A5           | Comunicação I2C     |
| RTC SDA      | A4           | I2C                 |
| RTC SCL      | A5           | I2C                 |
| LED Verde    | 8            | Status Ideal        |
| LED Amarelo  | 9            | Status Atenção      |
| LED Vermelho | 10           | Status Alerta       |
| Buzzer       | 7            | Alertas sonoros     |

---


# 🔄 Fluxo Resumido do Sistema

```
[Sensores] → [Arduino] → [Processamento] → [LCD]
                                 ↓
                           [LEDs/Buzzer]
                                 ↓
                        [Monitor Serial]
                                 ↓
                        [Dashboard BI]
```

---

# 📝 Observações Técnicas

* O ajuste do RTC deve ser habilitado **somente uma vez**
* Os limites ambientais podem ser ajustados no início do código
* O buzzer opera com padrões diferentes para cada tipo de alerta
* O sistema está preparado para futura integração com redes IoT reais (WiFi/MQTT)

