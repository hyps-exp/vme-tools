import requests
import sys

def send_to_influxdb(line_protocol): #vme_address, channel, enable, threshold, width):
  influxdb_url = 'http://172.16.205.212:8086/api/v2/write'
  bucket = 'threshold'
  org_name = 'hyps'
  auth_token = '-iT93bQ-4tDWCQVR42vBoRbE58wzxIWsDYB6S4vfgR9BiiRSrfoR90OpaYXtaBuEkAPgfrv0YrFtqQPFFn81Vg=='

  # line_protocol = (
  #   f'v895,vme_address={vme_address:x},channel={channel:02d} '
  #   f'enable={enable}i,vth={threshold}i,width={width}i'
  # )
  headers = {
    'Authorization': f'Token {auth_token}',
    'Content-Type': 'text/plain',
  }
  params = {
    'org': org_name,
    'bucket': bucket,
    'precision': 's',
  }
  response = requests.post(
    influxdb_url, headers=headers, params=params, data=line_protocol
  )
  if response.status_code == 204:
    print('Data sent to InfluxDB successfully.')
  else:
    print(f'Failed to send data to InfluxDB: {response.status_code}')
    print('Response:', response.text)

if __name__ == '__main__':
  # if len(sys.argv) != 6:
  #   print(f'Usage: python send_to_influxdb.py <vme_address> <channel> <enable> <threshold> <width>')
  #   sys.exit(1)

  # vme_address = sys.argv[1]
  # channel = int(sys.argv[2])
  # enable = int(sys.argv[3])
  # threshold = int(sys.argv[4])
  # width = int(sys.argv[5])

  # send_to_influxdb(vme_address, channel, enable, threshold, width)
  send_to_influxdb(sys.argv[1])
