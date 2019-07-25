

<!-- PROJECT LOGO -->
<br />
<p align="center">
    <img src="music.jpg" alt="Logo">
  </a>

  <h3 align="center">Speed Limit Reader</h3>

  <p align="center">
   This project utilize OpenCV and image processing operations to detect a speed limit sign in an inputted image, and then read and return the associated speed limit value. 
    <br />
    <br />
    Look bellow to see how it was made and what the results where!
    <br />
    <br />
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents
* [About The Project](#about-the-project)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Usage](#usage)
* [Image Processing Operations Used](#image-processing-operations-used)
* [Results](#results)
* [Contact](#contact)




<!-- ABOUT THE PROJECT -->
## About The Project
The concept behind this project is that it could theoretically be used for a self-driving vehicle, to detect and read speed signs. Of-course most self driving cars used stereo vision to see the roads, so this app is created more for fun then for real world use.

The project is currently in its beta stages. As of now the application can detect a stop sign, a 40km/hr sign and a 80km/hr sign. The goal is for the application to be able to detect all speed signs and this step requires a few more adjustments. 

That being said the core image processing procedures have been completed and its is simply a process of changing the method in which these techniques are used!

To learn more about how these strategies were used, check out the "Image Processing Operations Used" Section bellow.




<!-- GETTING STARTED -->
## Getting Started
I made the code public so others can poke around and experiment, so please clone and explore the code!



### Prerequisites

The program is written in C++ with OpenCV3.2 so before you do anything else, please go ahead and make sure you got both of these on your computer.

Also, for best results please use Microsoft Visual Studio, it is what I used to build and run the code and it is what you should use.





<!-- USAGE EXAMPLES -->
### Usage
To open the code and start coding, click the sln file, which should open VS Studio (seems obvious but I like it include this step just incase a noob is brave enough to play around with this code).

Currently the to input your own street images you will need to save them into the OpenCVExampleVC15-3.2\OpenCVExample\speedlimit folder and hard-code it being read into the file in the main. This of-course will be much smoother closer to the final build.

    ...
    //Step Zero: Load an image from file
	src = imread("speedlimit/speedsign13.jpg", 1);  ///your image
	...


<!--  image-processing-operations-used -->
## Image Processing Operations Used

COMING SOON


<!-- results -->
## Results
COMING SOON


<!-- CONTACT -->
## Contact

Luke Daschko - [@lukeduker](https://www.instagram.com/lukeduker/) - daschko.l@gmail.com

Project Link: [https://github.com/LukeVibes/SpeedLimitReader](https://github.com/LukeVibes/SpeedLimitReader)









<!-- MARKDOWN LINKS & IMAGES -->
[build-shield]: https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square
[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[license-shield]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: https://raw.githubusercontent.com/othneildrew/Best-README-Template/master/screenshot.png
