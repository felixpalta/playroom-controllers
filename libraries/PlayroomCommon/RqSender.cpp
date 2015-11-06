// 
// 
// 

#include "RqSender.h"
#include "playroom-server-address.h"

bool RqSender::get_connect_msg(int code, const char **msg)
{
  static const int SUCCESS = 1;
  static const int TIMED_OUT = -1;
  static const int INVALID_SERVER = -2;
  static const int TRUNCATED = -3;
  static const int INVALID_RESPONSE = -4;

  switch (code)
  {
  case SUCCESS:
    *msg = "Connection OK";
    return true;
  case TIMED_OUT:
    *msg = "Connection timed out";
    return false;
  case INVALID_SERVER:
    *msg = "Invalid server";
    return false;
  case TRUNCATED:
    *msg = "Truncated";
    return false;
  case INVALID_RESPONSE:
    *msg = "Invalid response";
    return false;
  default:
    *msg = "Unknown code";
    return false;
  }
}

bool RqSender::connect_to_server()
{
  int code;
  const char *err_msg = NULL;
  bool connect_ok = false;

  if (DNS_SERVER_NAME_PRESENT)
  {
    ok_printer.print("Connecting to server by DNS name: ");
    ok_printer.println(PLAYROOM_SERVER_DNS_ADDRESS);
    code = client.connect(PLAYROOM_SERVER_DNS_ADDRESS, PLAYROOM_SERVER_LISTENING_PORT);
    ok_printer.print("Code: ");
    ok_printer.println(code);
    connect_ok = get_connect_msg(code, &err_msg);
    if (!connect_ok)
    {
      Serial.println("Connection to server by DNS name failed");
      if (err_msg)
        Serial.println(err_msg);
    }
  }

  if (!connect_ok)
  {
    ok_printer.print("Connecting to server by IP address: ");
    ok_printer.println(PLAYROOM_SERVER_IP_ADDRESS);
    code = client.connect(PLAYROOM_SERVER_IP_ADDRESS, PLAYROOM_SERVER_LISTENING_PORT);
    ok_printer.print("Code: ");
    ok_printer.println(code);
    err_msg = NULL;
    if (!get_connect_msg(code, &err_msg))
    {
      Serial.println("Connection to server by IP address failed");
      if (err_msg)
        Serial.println(err_msg);
      return false;
    }
  }

  ok_printer.println("Connection to server successful");

  return true;
}

void RqSender::send_request(OutRqType rq_type, int n)
{
  if (!connect_to_server())
  {
    return;
  }
  ok_printer.println("Connected to server");
  if (!client.connected())
  {
    Serial.println("Error: server instantly closed the connection");
    client.stop();
    return;
  }

  switch (rq_type)
  {
  case OUT_RQ_TYPE_BARREL_PLAY:
    out_writer.send_barrel_play_request();
    break;
  case OUT_RQ_TYPE_BARREL_SECTOR:
    out_writer.send_barrel_sector_request(n);
    break;
  case OUT_RQ_TYPE_GAME_START:
    out_writer.send_game_start_request();
    break;
  case OUT_RQ_TYPE_STANDBY:
    out_writer.send_standby_request();
    break;
  case OUT_RQ_TYPE_CLEANING:
    out_writer.send_cleaning_request();
    break;
  default:
    Serial.print("Unexpected request type: ");
    Serial.println(rq_type);
    return;
  }

  if (!client.connected())
  {
    Serial.println("Error: server closed connection right after receiving from client");
    client.stop();
    return;
  }
  Serial.println("Raw response from server: ");

  last_read_time = millis();

  while (true)
  {
    if (client.available())
    {
      last_read_time = millis();
      char c = client.read();
      Serial.print(c);
    }
    if (millis() - last_read_time >= READ_TIMEOUT)
    {
      Serial.println("\nRead timeout!");
      break;
    }
  }
  if (!client.connected())
  {
    ok_printer.println("OK: Response received, server closed the connection");
  }
  else
  {
    Serial.println("FAIL: Response received, but server didn't close the connection");
  }
  ok_printer.println("Stopping the client...");
  client.stop();
  return;
}
