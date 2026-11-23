## Logon sem senha

1. Abra o arquivo /etc/lightdm/lightdm.conf e identifique a entrada #autologin-user=.
2. Remova o # e insira o nome do usuário:
autologin-user=caninos

APós a alteração, reinicie o computador.

## Boot pelo cartão:

1. Baixar e instalar com BalenaEtcher
2. Depois na partição BOOT do cartão SD criado, abrir o arquivo uEnv.txt e modificar root=/dev/mmcblk2p2 por root=/dev/mmcblk0p2.
3. Colocar o cartão na Labrador e ligar. Agora ela deve iniciar pelo cartão SD.

## Gravação de novo OS na eMMC:
Após iniciar pelo cartão SD, no terminal digitar:

$ ls
$ cd install 
$ sudo ./install32_labrador.sh

>> Observação:
 Corrigir a resolução do monitor para 1024x720 usando a ferramenta gráfica:
 $ lxrandr

## Automatizar conexão Bluetooth Labrador-ESP32

#### Passo 1: Script de ativação do hardware e pareamento

$ sudo nano ~/bin/btautoconnect.sh

Substitua o MAC pelo endereço do dispositivo:

#!/bin/bash
#Garante que o rfkill não esteja bloqueando o hardware
sudo rfkill unblock bluetooth

#Liga a energia do controlador Bluetooth principal
sudo bluetoothctl power on

#Opcional: Se quiser conectar automaticamente a um dispositivo específico (ex: fone, teclado)
MAC_DEVICE="AA:BB:CC:DD:EE:FF"
sudo bluetoothctl connect $MAC_DEVICE

Feche o arquivo e salve.

Dê permissão de execução ao script:

$chmod +x ~/bin/btautoconnect.sh

#### Passo 2: Forçar o Bluetooth a iniciar ligado nativamente

Abra o arquivo de configuração:

$sudo nano /etc/bluetooth/main.conf

Procure pela linha AutoEnable (geralmente no final do arquivo) e mude para true:

AutoEnable=true

#### Passo 3: Automatizar no Boot (via SystemD)

Crie o arquivo do serviço:

$sudo nano /etc/systemd/system/bluetooth-auto.service


Insira a configuração abaixo (ajuste o caminho /home/caninos/... de acordo com o seu usuário padrão da Labrador):

[Unit]
Description=Inicializar Bluetooth Automaticamente na Labrador
After=bluetooth.service
Requires=bluetooth.service

[Service]
Type=oneshot
ExecStart=/home/caninos/bin/btautoconnect.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target

Atualize o SystemD e ative o serviço para iniciar em todos os boots:

$sudo systemctl daemon-reload
$sudo systemctl enable bluetooth-auto.service

> Reinicie a Labrador