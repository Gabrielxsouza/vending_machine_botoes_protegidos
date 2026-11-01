# Vending Machine ESP32 — proteção paliativa para GPIO34/35

Projeto em C / ESP-IDF 5.5.x. Código recuperado da versão anterior da Vending Machine, mantendo a seleção por ADC, LEDs, saldos e preços, buzzer PWM e lógica de compra.

## O que foi alterado

- Substituídas as variáveis `last_coin_reading`, `coin_button_state`, `last_buy_reading`, `buy_button_state` e suas funções de debounce por um detector com estados separados para cada botão.
- `detectar_acionamento()` só retorna `true` depois desta sequência: (1) botão solto em HIGH por 800 ms, (2) botão pressionado em LOW por 500 ms, (3) botão solto em HIGH por 250 ms. As trocas de nível são previamente estabilizadas por 70 ms.
- **A ação acontece quando você SOLTA o botão**, não no instante em que começa a apertar.
- O botão precisa ficar solto antes de cada novo acionamento. O sistema não conta vários pressionamentos durante um único LOW contínuo.
- A pinagem foi mantida: GPIO34 moeda; GPIO35 compra; GPIO32 ADC; GPIO27/14/12 LEDs; GPIO2 buzzer.
- A lógica da máquina continua igual: produtos de 1, 2 ou 3 moedas e sons de feedback.

## Limitação física essencial

**GPIO34 e GPIO35 do ESP32 clássico não possuem resistores internos de pull-up nem pull-down.** Se a PCB não tem resistores de polarização, os pinos podem flutuar e produzir leituras falsas mesmo com essa lógica. Não existe algoritmo que garanta detectar um pressionamento real em um sinal eletricamente indefinido. Esta versão serve como teste/paliativo, **não é solução definitiva**.

Se o circuito de botão for ativo-baixo (botão ligando sinal ao GND), a correção confiável é conectar **um resistor de 10 kΩ entre cada sinal GPIO34/GPIO35 e 3,3 V**. Desligue o hardware antes de qualquer intervenção. Nunca ligue 5 V ao GPIO.

## Troco e saldo salvo

- **Troco automático (simulado):** se o saldo for maior que o preço, depois da compra a diferença é devolvida. Cada moeda devolvida gera 1 piscada do LED do produto + 1 bipe, e o saldo volta a 0. Não há hardware de devolução de moedas; para ter um, basta acioná-lo dentro de `return_change()` em `vending.c`.
- **Saldo na flash (NVS):** o saldo é gravado a cada mudança (moeda inserida, compra, cada moeda de troco). No boot, o saldo salvo é restaurado e aparece `[i] Saldo restaurado da memoria`.
- Na compra, o preço é debitado e salvo **antes** de dispensar o produto. Se a energia cair durante o troco, somente as moedas que ainda não foram devolvidas continuam como crédito.
- Para zerar o saldo salvo: `idf.py erase-flash` (apaga toda a flash; depois grave o firmware de novo).

## Estrutura do código (pasta main/)

| Arquivo | Responsabilidade |
|---|---|
| `pinos.h` | **Mapa de pinos** e recursos (canal ADC, timer/canal LEDC). Para mudar a pinagem, edite só este arquivo |
| `hardware.c/.h` | **Configuração** de todos os GPIOs, do ADC e do PWM do buzzer (`hardware_init()`) |
| `config.h` | Parâmetros de software ajustáveis (tempos dos botões, troco, NVS) |
| `utils.c/.h` | `delay_ms()` e `millis_now()` |
| `leds.c/.h` | LEDs indicadores de produto |
| `buzzer.c/.h` | Buzzer PWM e sons de feedback |
| `seletor.c/.h` | Potenciômetro (ADC) para seleção do produto |
| `botao.c/.h` | Detector robusto de acionamento (debounce + máquina de estados) |
| `produtos.c/.h` | Catálogo: preço, LED e nome de cada produto |
| `persistencia.c/.h` | Salva/carrega o saldo na flash (NVS) |
| `vending.c/.h` | Lógica da máquina: saldo, seleção e compra |
| `main.c` | `app_main()`: inicialização e loop principal |

Ao criar um novo `.c`, adicione-o em `SRCS` no `main/CMakeLists.txt`.

## Ajustes possíveis (main/config.h)

- `BTN_DEBOUNCE_MS = 70`: estabilidade de cada nível digital;
- `BTN_ARM_HIGH_MS = 800`: tempo de repouso antes de aceitar um novo botão;
- `BTN_HOLD_LOW_MS = 500`: tempo mínimo segurando o botão;
- `BTN_RELEASE_HIGH_MS = 250`: tempo de liberação para confirmar;
- `BTN_DEBUG = 1`: mostra níveis brutos e estados a cada 500 ms para diagnóstico.

Observe que **os intervalos são aproximados**, pois o loop tem pausas para o buzzer e a animação de compra.

## Como instalar e testar

1. Abra a pasta raiz do projeto (que contém o CMakeLists.txt principal) no VS Code.
2. No terminal do ESP-IDF, rode `idf.py set-target esp32` (caso ainda não esteja definido) e `idf.py build`.
3. Com a porta serial livre, rode `idf.py -p COM4 flash monitor` — mude COM4 se a porta da sua placa for outra.
4. Deixe os botões soltos após o boot; depois pressione e segure o botão por pelo menos 0,5 s, solte e aguarde a ação.
5. Experimente deixar 2 minutos sem tocar na PCB. Se contabilizar moedas ou comprar sozinho, o problema elétrico permanece e precisa ser corrigido fisicamente.

## Cuidados

- Os botões continuam sendo lidos por **polling a cada 10 ms**; não foram convertidos para interrupções.
- Eventos podem não ser reconhecidos durante animações bloqueantes de compra; essa limitação já existia na versão original.
- Os GPIO12 e GPIO2 são pinos de strapping e exigem cuidado com as conexões no boot.
- O código não foi compilado dentro de um ambiente ESP-IDF completo nem testado em uma placa neste ambiente.
