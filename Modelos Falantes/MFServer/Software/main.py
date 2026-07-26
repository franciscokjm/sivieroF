"""
    Nome do Projeto: MFServer - Modelos Falantes Server                                  25/07/2026

    Pesquisador Responsável: Prof. Fabio Siviero - siviero@usp.br
    Desenvolvimento Técnico: Dr. Kelliton Jose Mendonca Francisco - kelliton@usp.br
    Pesquisadores: Prof. Dr. Paulo Eduardo Capel Cardoso - paulocapel@usp.br
    Instituição de Origem: ICB - Universidade de São Paulo (USP)

    Ambiente de desenvolvimento: Visual Studio Code version 1.96.4
    Hardware: Single Board Computer Caninos Loucos Labrador 64 bits
    Software: Python 3.11.6, influxdb-client
    
    Este arquivo é um software de código aberto, licenciado sob a Licença GNU.
    Veja o arquivo LICENSE na raiz do projeto para mais detalhes.

    Este este subprojeto faz parte dos trabalhos desenvolvidos no âmbito do 
    Centro de Pesquisa e Orientação sobre Deficiência Visual (CpodV) financiado pela 
    Fundação de Apoio a Pesquisa do Estado de São Paulo e com participação 
    da Secretaria de Estado dos Direitos da Pessoa com Deficiência.
"""

# >>>>>>>>>>>>> CODIGO EM DESENVOLVIMENTO <<<<<<<<<<<

import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "<my-bucket>"
org = "<my-org>"
token = "<my-token>"
# Store the URL of your InfluxDB instance
url="http://localhost:8086"

client = influxdb_client.InfluxDBClient(
   url=url,
   token=token,
   org=org
)

write_api = client.write_api(write_options=SYNCHRONOUS)

p = influxdb_client.Point("my_measurement").tag("location", "Prague").field("temperature", 25.3)
write_api.write(bucket=bucket, org=org, record=p)