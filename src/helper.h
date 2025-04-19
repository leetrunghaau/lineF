#include <Arduino.h>

String converseLine(int lines[])
{
  String json = "{\"type\":\"line\",\"data\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lines[i]);
    if (i < 4)
      json += ",";
  }
  json += "]}";
  return json;
}

String converseConnect(int lowLine[], int highLine[], int line[])
{
  String json = "{\"type\":\"connect\",\"data\":{";

  // lowLine
  json += "\"lowLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lowLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // highLine
  json += "\"highLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(highLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // line
  json += "\"line\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(line[i]);
    if (i < 4)
      json += ",";
  }
  json += "]}}";

  return json;
}
