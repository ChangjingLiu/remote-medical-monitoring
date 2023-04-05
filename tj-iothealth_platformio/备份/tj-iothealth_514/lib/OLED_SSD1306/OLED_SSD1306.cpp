//#ifndef _OLED_SSD1306_H_
#include "OLED_SSD1306.h"
//#endif
#include <Arduino.h>

//定义字号大小
#define size1

#define ch_size 16

#ifdef size1
#define size_w 6
#define size_h 8
#endif

#ifdef size2
#define size_w 12
#define size_h 16
#endif

#define x_hm SCREEN_WIDTH - 5 * size_w //定义时间的起始x坐标
#define y_time 3                       //定义时间栏的y坐标
#define y_info 48                      //定义信息栏的y坐标

/*--  文字:  系  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM xi[]{
    0x00, 0xF8, 0x3F, 0x00, 0x04, 0x00, 0x08, 0x20, 0x10, 0x40, 0x3F, 0x80, 0x01, 0x00, 0x06, 0x10,
    0x18, 0x08, 0x7F, 0xFC, 0x01, 0x04, 0x09, 0x20, 0x11, 0x10, 0x21, 0x08, 0x45, 0x04, 0x02, 0x00};

/*--  文字:  统  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM tong[]{
    0x10, 0x40, 0x10, 0x20, 0x20, 0x20, 0x23, 0xFE, 0x48, 0x40, 0xF8, 0x88, 0x11, 0x04, 0x23, 0xFE,
    0x40, 0x92, 0xF8, 0x90, 0x40, 0x90, 0x00, 0x90, 0x19, 0x12, 0xE1, 0x12, 0x42, 0x0E, 0x04, 0x00};

/*--  文字:  启  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM qi[]{
    0x01, 0x00, 0x00, 0x80, 0x1F, 0xFC, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x1F, 0xFC, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x17, 0xFC, 0x24, 0x04, 0x24, 0x04, 0x44, 0x04, 0x87, 0xFC, 0x04, 0x04};

/*--  文字:  动  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM dong[]{
    0x00, 0x40, 0x00, 0x40, 0x7C, 0x40, 0x00, 0x40, 0x01, 0xFC, 0x00, 0x44, 0xFE, 0x44, 0x20, 0x44,
    0x20, 0x44, 0x20, 0x84, 0x48, 0x84, 0x44, 0x84, 0xFD, 0x04, 0x45, 0x04, 0x02, 0x28, 0x04, 0x10};

/*--  文字:  中  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM zhong[]{
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x3F, 0xF8, 0x21, 0x08, 0x21, 0x08, 0x21, 0x08,
    0x21, 0x08, 0x21, 0x08, 0x3F, 0xF8, 0x21, 0x08, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00};

static const unsigned char PROGMEM nian[] =
    {0x10, 0x00, 0x10, 0x00, 0x1F, 0xFC, 0x20, 0x80, 0x20, 0x80, 0x40, 0x80, 0x1F, 0xF8, 0x10, 0x80,
     0x10, 0x80, 0x10, 0x80, 0xFF, 0xFE, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80};

static const unsigned char PROGMEM yue[] =
    {
        0x00, 0x00, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08,
        0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08, 0x20, 0x08, 0x20, 0x08, 0x40, 0x28, 0x80, 0x10};

static const unsigned char PROGMEM ri[] =
    {
        0x00, 0x00, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08,
        0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x10, 0x08, 0x20, 0x08, 0x20, 0x08, 0x40, 0x28, 0x80, 0x10};

//中文：心
static const unsigned char PROGMEM xin[] =
    {
        0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x80, 0x04, 0x00, 0x04, 0x08, 0x24, 0x04,
        0x24, 0x04, 0x24, 0x02, 0x44, 0x02, 0x44, 0x12, 0x84, 0x10, 0x04, 0x10, 0x03, 0xF0, 0x00, 0x00};
//中文：率
static const unsigned char PROGMEM lv[] =
    {
        0x02, 0x00, 0x01, 0x00, 0x7F, 0xFC, 0x02, 0x00, 0x44, 0x44, 0x2F, 0x88, 0x11, 0x10, 0x22, 0x48,
        0x4F, 0xE4, 0x00, 0x20, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00};

/*--  文字:  温  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM wen[] =
    {
        0x00, 0x00, 0x23, 0xF8, 0x12, 0x08, 0x12, 0x08, 0x83, 0xF8, 0x42, 0x08, 0x42, 0x08, 0x13, 0xF8,
        0x10, 0x00, 0x27, 0xFC, 0xE4, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x24, 0xA4, 0x2F, 0xFE, 0x00, 0x00};

/*--  文字:  度  --*/
/*--  Fixedsys12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM du[] =
    {
        0x01, 0x00, 0x00, 0x80, 0x3F, 0xFE, 0x22, 0x20, 0x22, 0x20, 0x3F, 0xFC, 0x22, 0x20, 0x22, 0x20,
        0x23, 0xE0, 0x20, 0x00, 0x2F, 0xF0, 0x24, 0x10, 0x42, 0x20, 0x41, 0xC0, 0x86, 0x30, 0x38, 0x0E};

/*--  文字:  血  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM xue[] =
    {
        0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x3F, 0xF8, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48,
        0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0xFF, 0xFE, 0x00, 0x00};

/*--  文字:  氧  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM yang[] =
    {
        0x20, 0x00, 0x3F, 0xFC, 0x40, 0x00, 0x9F, 0xF0, 0x00, 0x00, 0x7F, 0xF0, 0x11, 0x10, 0x0A, 0x10,
        0x7F, 0xD0, 0x04, 0x10, 0x3F, 0x90, 0x04, 0x12, 0xFF, 0xEA, 0x04, 0x0A, 0x04, 0x06, 0x04, 0x02};

/*--  文字:  信  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM xin4[] =
    {
        0x08, 0x40, 0x08, 0x20, 0x0B, 0xFE, 0x10, 0x00, 0x10, 0x00, 0x31, 0xFC, 0x30, 0x00, 0x50, 0x00,
        0x91, 0xFC, 0x10, 0x00, 0x10, 0x00, 0x11, 0xFC, 0x11, 0x04, 0x11, 0x04, 0x11, 0xFC, 0x11, 0x04};

/*--  文字:  息  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/

static const unsigned char PROGMEM xi1[] =
    {
        0x01, 0x00, 0x02, 0x00, 0x1F, 0xF0, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10,
        0x1F, 0xF0, 0x10, 0x10, 0x01, 0x00, 0x08, 0x84, 0x48, 0x92, 0x48, 0x12, 0x87, 0xF0, 0x00, 0x00};

/*--  文字:  测  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM ce4[] =
    {
        0x00, 0x04, 0x27, 0xC4, 0x14, 0x44, 0x14, 0x54, 0x85, 0x54, 0x45, 0x54, 0x45, 0x54, 0x15, 0x54,
        0x15, 0x54, 0x25, 0x54, 0xE5, 0x54, 0x21, 0x04, 0x22, 0x84, 0x22, 0x44, 0x24, 0x14, 0x08, 0x08};

/*--  文字:  量  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM liang2[] =
    {
        0x00, 0x00, 0x1F, 0xF0, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0xFF, 0xFE, 0x00, 0x00, 0x1F, 0xF0,
        0x11, 0x10, 0x1F, 0xF0, 0x11, 0x10, 0x1F, 0xF0, 0x01, 0x00, 0x1F, 0xF0, 0x01, 0x00, 0x7F, 0xFC};

/*--  文字:  确  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM que4[] =
    {
        0x00, 0x40, 0x00, 0x40, 0xFC, 0x7C, 0x10, 0x84, 0x11, 0x08, 0x22, 0xFE, 0x3C, 0x92, 0x64, 0x92,
        0x64, 0xFE, 0xA4, 0x92, 0x24, 0x92, 0x24, 0xFE, 0x3C, 0x92, 0x25, 0x12, 0x21, 0x0A, 0x02, 0x04};

/*--  文字:  认  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/

static const unsigned char PROGMEM ren4[] =
    {
        0x00, 0x40, 0x20, 0x40, 0x10, 0x40, 0x10, 0x40, 0x00, 0x40, 0x00, 0x40, 0xF0, 0x40, 0x10, 0x40,
        0x10, 0xA0, 0x10, 0xA0, 0x10, 0xA0, 0x15, 0x10, 0x19, 0x10, 0x12, 0x08, 0x04, 0x04, 0x08, 0x02};

/*--  文字:  返  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM fan3[] =
    {
        0x00, 0x08, 0x20, 0x3C, 0x13, 0xC0, 0x12, 0x00, 0x02, 0x00, 0x03, 0xFC, 0xF2, 0x04, 0x12, 0x88,
        0x12, 0x50, 0x12, 0x20, 0x12, 0x50, 0x14, 0x88, 0x15, 0x04, 0x28, 0x00, 0x47, 0xFE, 0x00, 0x00};

/*--  文字:  回  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM hui2[] =
    {
        0x00, 0x00, 0x3F, 0xF8, 0x20, 0x08, 0x20, 0x08, 0x27, 0xC8, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48,
        0x24, 0x48, 0x24, 0x48, 0x27, 0xC8, 0x20, 0x08, 0x20, 0x08, 0x3F, 0xF8, 0x20, 0x08, 0x00, 0x00};

/*--  文字:  配  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM pei4[] =
    {
        0x00, 0x00, 0xFE, 0x00, 0x28, 0xF8, 0x28, 0x08, 0xFE, 0x08, 0xAA, 0x08, 0xAA, 0x08, 0xAA, 0xF8,
        0xAE, 0x88, 0xC2, 0x80, 0x82, 0x80, 0xFE, 0x80, 0x82, 0x82, 0x82, 0x82, 0xFE, 0x7E, 0x82, 0x00};

/*--  文字:  置  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
static const unsigned char PROGMEM zhi4[] =
    {
        0x7F, 0xFC, 0x44, 0x44, 0x7F, 0xFC, 0x01, 0x00, 0x7F, 0xFC, 0x01, 0x00, 0x1F, 0xF0, 0x10, 0x10,
        0x1F, 0xF0, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0xFF, 0xFE, 0x00, 0x00};

OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin) : Adafruit_SSD1306(w, h, twi, rst_pin){
                                                                                     //OLED::Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, wire, -1, 400000UL, 100000UL);
                                                                                 };
/*
OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                           int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
{
}

OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                           int8_t rst_pin, int8_t cs_pin, uint32_t bitrate) : Adafruit_SSD1306(w, h, spi, dc_pin, rst_pin, cs_pin, bitrate)
{
}

// DEPRECATED CONSTRUCTORS - for back compatibility, avoid in new projects
OLED_SSD1306::OLED_SSD1306(int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin,
                           int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
{
}
OLED_SSD1306::OLED_SSD1306(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(dc_pin, rst_pin, cs_pin)
{
}
OLED_SSD1306::OLED_SSD1306(int8_t rst_pin) : Adafruit_SSD1306(rst_pin) {}
*/
/*
OLED_SSD1306::~OLED_SSD1306(void)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}
*/
/*
OLED::OLED(uint8_t w, uint8_t h, TwoWire *twi,
           int8_t rst_pin, uint32_t clkDuring,
           uint32_t clkAfter)
{
    //OLED::Adafruit_SSD1306()
    OLED::Adafruit_SSD1306(w, h, twi, -1, 400000UL, 100000UL);
}
*/

OLED_SSD1306::~OLED_SSD1306(void)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}

void OLED_SSD1306::Initial(void)
{

    if (!OLED_SSD1306::begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            //yield();
            ; // Don't proceed, loop forever
    }
    Serial.println("SSD1306 ok");
    //delay(2000);
    OLED_SSD1306::clearDisplay();
    OLED_SSD1306::drawBitmap(0 * ch_size, 24, xi, 16, 16, 1);    //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(1 * ch_size, 24, tong, 16, 16, 1);  //在坐标X:42  Y:16的位置显示中文字符率
    OLED_SSD1306::drawBitmap(2 * ch_size, 24, qi, 16, 16, 1);    //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(3 * ch_size, 24, dong, 16, 16, 1);  //在坐标X:42  Y:16的位置显示中文字符率
    OLED_SSD1306::drawBitmap(4 * ch_size, 24, zhong, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::setCursor(5 * ch_size, 24);                    //起点坐标
    OLED_SSD1306::setTextSize(2);                                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println("...");
    OLED_SSD1306::display();
    delay(3000);
};

void OLED_SSD1306::drawtime(NTPClient timeClient, lasttime *ltime)
{
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime(); //获取EpochTime
    delay(100);
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int years = ptm->tm_year + 1900; //若此时为1970
    int printyears;
    if (years == 1970)
    {
        printyears = ltime->years;
    }
    else
    {
        printyears = years;
    }
    //int months = ptm->tm_mon + 1;
    //int days = ptm->tm_mday;
    String monthsStr;
    String daysStr;
    if (years == 1970)
    {
        monthsStr = ltime->monthsStr;
        daysStr = ltime->daysStr;
    }
    else
    {
        monthsStr = ptm->tm_mon + 1 < 10 ? "0" + String(ptm->tm_mon + 1) : String(ptm->tm_mon + 1);
        daysStr = ptm->tm_mday + 1 < 10 ? "0" + String(ptm->tm_mday) : String(ptm->tm_mday);
    }
    //int

    OLED_SSD1306::setCursor(0 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);   //字体颜色为白色
    OLED_SSD1306::println(printyears);           //年份
    //OLED_SSD1306::drawBitmap(48, 3, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 年
    OLED_SSD1306::setCursor(4 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);   //字体颜色为白色
    OLED_SSD1306::println("-");

    //月份
    OLED_SSD1306::setCursor(5 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);   //字体颜色为白色
    OLED_SSD1306::println(monthsStr);            //月份

    OLED_SSD1306::setCursor(7 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);   //字体颜色为白色
    OLED_SSD1306::println("-");

    //日
    OLED_SSD1306::setCursor(8 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);   //字体颜色为白色
    OLED_SSD1306::println(daysStr);              //月份

    //时间
    int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();
    String hoursStr;
    String minuteStr;
    if (years == 1970)
    {
        hoursStr = ltime->hoursStr;
        minuteStr = ltime->minuteStr;
    }
    else
    {
        hoursStr = hours < 10 ? "0" + String(hours) : String(hours);
        minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);
    }
    OLED_SSD1306::setCursor(x_hm, y_time);     //起点坐标
    OLED_SSD1306::setTextSize(1);              // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE); //字体颜色为白色
    OLED_SSD1306::println(hoursStr);           //小时

    OLED_SSD1306::setCursor(x_hm + 2 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                       // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);          //字体颜色为白色
    OLED_SSD1306::println(":");

    OLED_SSD1306::setCursor(x_hm + 3 * size_w, y_time); //起点坐标
    OLED_SSD1306::setTextSize(1);                       // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);          //字体颜色为白色
    OLED_SSD1306::println(minuteStr);

    ltime->years = years;
    ltime->monthsStr = monthsStr;
    ltime->daysStr = daysStr;
    ltime->hoursStr = hoursStr;
    ltime->minuteStr = minuteStr;
    //String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    //OLED_SSD1306::setCursor(64, 3);                    //起点坐标
    //OLED_SSD1306::println(ptm->tm_mon + 1);           //月份
    //OLED_SSD1306::drawBitmap(28, 3, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 月
    //OLED_SSD1306::println(ptm->tm_mday);              //日
    //OLED_SSD1306::drawBitmap(30, 0, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 月

    //int monthDay = ptm->tm_mday;
    //Serial.print("Month day: ");
    //Serial.println(monthDay);
    //int currentMonth = ptm->tm_mon + 1;
    //Serial.print("Month: ");
    //Serial.println(currentMonth);
    //String currentMonthName = months[currentMonth - 1];
    //Serial.print("Month name: ");
    //Serial.println(currentMonthName);
    //int currentYear = ptm->tm_year + 1900;
    //Serial.print("Year: ");
    //Serial.println(currentYear);

    //OLED_SSD1306::display();
    delay(100);
};

void OLED_SSD1306::drawBodyTemperature(float BodyTemperature)
{
    //OLED_SSD1306::clearDisplay();

    //display.setTextSize(2);             // Normal 1:1 pixel scale
    //display.setTextColor(SSD1306_WHITE);        // Draw white text
    //display.setCursor(0,0);             // Start at top-left corner

    OLED_SSD1306::drawBitmap(0, 12, wen, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(16, 12, du, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

    OLED_SSD1306::setCursor(32, 14); //起点坐标
    OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println(":");

    OLED_SSD1306::setCursor(38, 14); //起点坐标
    OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    char temp[6];
    if (BodyTemperature)
    {
        sprintf(temp, "%.1f", BodyTemperature);
        OLED_SSD1306::println(temp);
    }

    //OLED_SSD1306::setCursor(62, 14); //起点坐标
    //OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    //OLED_SSD1306::setTextColor(SSD1306_WHITE);
    //OLED_SSD1306::println("C");

    //OLED_SSD1306::display();
    delay(100);
};

void OLED_SSD1306::drawSpO2(float SpO2)
{
    //OLED_SSD1306::clearDisplay();

    //display.setTextSize(2);             // Normal 1:1 pixel scale
    //display.setTextColor(SSD1306_WHITE);        // Draw white text
    //display.setCursor(0,0);             // Start at top-left corner

    OLED_SSD1306::drawBitmap(64, 12, xue, 16, 16, 1);  //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(80, 12, yang, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

    OLED_SSD1306::setCursor(96, 14); //起点坐标
    OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println(":");

    if (SpO2 > 0)
    {
        OLED_SSD1306::setCursor(102, 14); //起点坐标
        OLED_SSD1306::setTextSize(1, 2);  // Draw 2X-scale text
        OLED_SSD1306::setTextColor(SSD1306_WHITE);
        char temp[6];
        sprintf(temp, "%.1f", SpO2);
        OLED_SSD1306::println(temp);
    }

    /*
    OLED_SSD1306::setCursor(62, 14); //起点坐标
    OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println("C");
*/
    //OLED_SSD1306::display();
    delay(100);
};

void OLED_SSD1306::drawHeartRate(int HR)
{
    //OLED_SSD1306::clearDisplay();

    //display.setTextSize(2);             // Normal 1:1 pixel scale
    //display.setTextColor(SSD1306_WHITE);        // Draw white text
    //display.setCursor(0,0);             // Start at top-left corner

    OLED_SSD1306::drawBitmap(0, 30, xin, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(16, 30, lv, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

    OLED_SSD1306::setCursor(32, 30); //起点坐标
    OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println(":");

    if (HR > 0)
    {
        OLED_SSD1306::setCursor(38, 32); //起点坐标
        OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
        OLED_SSD1306::setTextColor(SSD1306_WHITE);
        //char temp[6];
        //sprintf(temp, "%.1f", HR);
        OLED_SSD1306::println(HR);
    }

    //OLED_SSD1306::setCursor(62, 14); //起点坐标
    //OLED_SSD1306::setTextSize(1, 2); // Draw 2X-scale text
    //OLED_SSD1306::setTextColor(SSD1306_WHITE);
    //OLED_SSD1306::println("C");

    //OLED_SSD1306::display();
    delay(100);
};

void OLED_SSD1306::drawInfo(void)
{
    OLED_SSD1306::drawBitmap(0, y_info, xin4, 16, 16, 1); //在坐标X:0  Y:30的位置显示中文字符心
    OLED_SSD1306::drawBitmap(16, y_info, xi1, 16, 16, 1); //在坐标X:16  Y:30的位置显示中文字符率
    delay(50);
};
void OLED_SSD1306::drawMonitoring(void)
{
    OLED_SSD1306::drawBitmap(31, y_info, ce4, 16, 16, 1);    //在坐标X:0  Y:30的位置显示中文字符心
    OLED_SSD1306::drawBitmap(48, y_info, liang2, 16, 16, 1); //在坐标X:16  Y:30的位置显示中文字符率
    delay(50);
};

void OLED_SSD1306::drawConfig(void)
{
    OLED_SSD1306::drawBitmap(65, y_info, pei4, 16, 16, 1);    //在坐标X:0  Y:30的位置显示中文字符心
    OLED_SSD1306::drawBitmap(81, y_info+1, zhi4, 16, 16, 1); //在坐标X:16  Y:30的位置显示中文字符率
    delay(50);
};

void OLED_SSD1306::drawConfirm(void)
{
    OLED_SSD1306::drawBitmap(96, y_info, que4, 16, 16, 1);  //在坐标X:0  Y:30的位置显示中文字符心
    OLED_SSD1306::drawBitmap(112, y_info, ren4, 16, 16, 1); //在坐标X:16  Y:30的位置显示中文字符率
    delay(50);
}
void OLED_SSD1306::drawBack(void)
{
    OLED_SSD1306::drawBitmap(96, y_info, fan3, 16, 16, 1);  //在坐标X:0  Y:30的位置显示中文字符心
    OLED_SSD1306::drawBitmap(112, y_info, hui2, 16, 16, 1); //在坐标X:16  Y:30的位置显示中文字符率
    delay(50);
}
void OLED_SSD1306::drawtan(int_fast32_t x)
{
    OLED_SSD1306::drawRect(x, 47, 32, 17, SSD1306_WHITE);
}