# ATtiny85_DHT11_IRsend
這是一個使用ATtiny85讀取DHT11並發送IR紅外線進行控制的電路

# 主要功能
偵測室內溫度，並將裝置設置在冷氣機遙控範圍內，當室內溫度高於指定溫度及自動遙控開啟冷氣機並且定時一小時

# 動機
- 半夜睡覺氣到一直被熱醒起床開冷氣
但如果要在Arduino上會很好實現，但是如果要用小體積的ATtiny85就需要透過我整理的該程式碼實現，主要問題有：
- Attiny85內部頻率需特別設定，才能發送IR code (詳見檔案內附上的lirary-attiny85_ir_send)
- 因為舊機型冷氣的IR code在錄製的時候發現特別長，因此用library正常方式無法實現功能會被截斷，顧透過我另外寫的python將所有IR code根據library的定義拆解成一行一行的Arduino code

# 所需材料
- [ ] ATtiny85
- [ ] DHT11
- [ ] IR LED
- [ ] 老舊型冷氣(型號有空補上)
- [ ] SW (button), LED, Resistor
