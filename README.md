<p float="left">

# 3D Scanner Prototype Project

<img src="https://user-images.githubusercontent.com/100609081/156028721-3555e2b3-4dd3-42df-afae-8f5c403dcdf7.png" width="500" /><img src="https://user-images.githubusercontent.com/100609081/234333493-02c30327-6e9e-4885-bb9b-d01275554971.png" width="400" height="663" />

## Project Description
### Components
The main components of the 3D scanner include:
- A rotating table where the object is placed.
- A movable carriage with a distance sensor.

### How It Works
#### Scanning Process:
1. The object is placed on the rotating table.
2. As the table rotates, the carriage with the sensor performs measurements.
3. After each rotation, the carriage moves upward, and the process repeats until the entire object is scanned.
#### Data Processing:
- Measurements are converted into Cartesian coordinates and saved on an SD card.

![image](https://github.com/Rothord/3DScanner/assets/100609081/faaa4fd7-e08d-4e57-8eb5-429d679de950)

## Challenges and Solutions
### Printing the Scanner Parts
- **Software Used:** Autodesk Inventor for 3D modeling.
- **Issues Encountered:** Initial prints were unsuccessful due to inexperience with 3D printing.
- **Solution:** With guidance from experienced individuals and through trial and error, the key components were successfully printed.

### Programming the Scanner
- **Initial Problem:** The table did not always complete full rotations, resulting in inaccurate scans.
- **Solution:** After extensive troubleshooting and adjustments to the code, the issue was resolved, allowing for the successful scanning of objects.
  
![image](https://github.com/Rothord/3DScanner/assets/100609081/ad5195af-4d42-489b-b2fe-b6881ca1dba5)

### Data Processing
- **Noise Reduction:** The measurement data were imported into Matlab. Using digital filters, noise caused by slight table tilts and measurement fluctuations was eliminated.
- **Further Processing:** The resulting point cloud was refined in Blender, a 3D modeling and animation software.
  
![image](https://github.com/Rothord/3DScanner/assets/100609081/9961c5ab-65c3-4658-ab07-95ad7e5dc123)

## Conclusion
Although the final result is not perfect, the project has been a success. It provided invaluable experience in 3D printing and Arduino microcontroller programming.

</p>
