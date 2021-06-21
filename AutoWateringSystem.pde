import controlP5.*;
import processing.serial.*;
ControlP5 cp5;
Serial port;

void setup(){
  size(400,600);//GUI boyutunun belirlenmesi
  //port = new Serial(this,"COM6",9600);//Arduino ile bağlantı sağlanabilmesini sağlamak için kullanılan kod
  PFont font = createFont("Times New Roman",30); // GUI için font ayarlanması
  cp5 = new ControlP5(this);
  cp5.setFont(font);
  cp5.addButton("PUMP_ON")//Pompayı çalıştırmak için kullanılan butonun oluşturulması
    .setPosition(0,0)
    .setSize(200,300)
    .setColorBackground(color(255,0,0,255))//renk ayarlamaları
    .setColorActive(color(255,37,37,255))
    .setColorForeground(color(255,137,137,255))
    
    ;
  cp5.addButton("PUMP_OFF")//Pompayı kapatmak için kullanılan butonun oluşturulması
    .setPosition(200,0)
    .setSize(200,300)
    .setColorBackground(color(0,0,255,255))//renk ayarlamaları
    .setColorActive(color(37,37,255,255))
    .setColorForeground(color(137,137,255,255))
    
    ;
  cp5.addButton("ON")
    .setPosition(0,300)
    .setSize(200,300)
    .setColorBackground(color(0,255,0,255))//Sistemi çalıştırmak için kullanılan butonun oluşturulması
    .setColorActive(color(37,255,37,255))
    .setColorForeground(color(137,255,137,255))
    
    
    ;
  cp5.addButton("OFF")
    .setPosition(200,300)
    .setSize(200,300)
    .setColorBackground(color(255,122,200,251))//Sistemi kapatmak için kullanılan butonun oluşturulması
    .setColorActive(color(255,150,230,251))
    .setColorForeground(color(255,180,222,251))
    
    
    ;
}

void draw(){
  background(color(255));

}

void PUMP_ON(){ //Pompayı çalıştırmak için kullanılan fonksiyon
  port.write('1');
}
void PUMP_OFF(){//Pompayı kapatmak için kullanılan fonksiyon
  port.write('0');
}
void ON(){//Sistemi çalıştırmak için kullanılan fonksiyon
  port.write('3');
}
void OFF(){//Sistemi kapatmak için kullanılan fonksiyon
  port.write('4');
}
