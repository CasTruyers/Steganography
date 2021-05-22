# STEGANOGRAPHY

**Steganography is the practice of hiding a secret message inside something that is not secret.**

*This program is writtin on a UNIX based machine and might not work on Windows*

### DESCRIPTION
This program can encrypt a text in an image. Thus, secrets messages can be sent between people with this program by message-encrypted images.
The same program can also decrypt the encrypted images to release the secret message.
The program is written in c and the file format in which encrypting is supported is .bmp (bitmap file)

### FEATURES
Reading .txt file and encrypting the message in .bmp image file.
Decrypting an encrypted .bmp image file and writing the secret message in .txt file.

### HOW TO INSTALL
Download "steganography.c" and compile it.

### HOW TO USE
To use the program you will need to pass arguments.

example:
to compress: ./program -c -s <secretMessageIn.txt> -i <inputImage> -o <outputImage>   
to decompress: ./program -d -i <inputImage> -o <secretMessageOut.txt>   

  -First argument "-c" or "-d" stands for compressing("-c") or decompressing ("-d").   
  * Choose if you want to encrypt(compress) or decrypt(decompress) a message.
    
  -s <filename/path>     
  * When compressing.  --> .txt file with message to compress in image.   
    
  -i <filename/path>  
  * When compressing   --> input .bmp image file to compress message in.     
  * when decompressing --> input bmp file with compressed message. 
    
  -o <filename/path> 
  * When compressing   --> output .bmp file with compressed message.    
  * When decompressing --> ouput .txt file with compressed message.     

***have fun***
