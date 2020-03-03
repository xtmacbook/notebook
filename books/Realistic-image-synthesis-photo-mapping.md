# Foreword  
The two breakthrough algorithms in image synthesis ware ** ray tracing ** and ** radiosity **. **Stochastic or distributed ray** tracing was invented to deal with motion blur,depth of filed and reflections from glossy surfaces,and path tracing extended the algorithm to deal with mutual interreflection.**Radiosity** takes a different approach: it assumes the world consists of diffuse surface patches,and the solves a matrix equation for the amount of light reaching each pach.  
  
Time has also indicated that the most general and robust approach is **Monte Carlo Ray Tracing**.But it has its problems.Recently a breakthrough occured: a subtle and simple method -**Photo Mapping** was invented by Henrik Wann Jensen to speedup **MCRT**(Monte Carlo Ray Tracing).The idea is to break ray tracing into two passes:the first castes photons into the scene from the light sources,and the second collects the photons to produce an image.

## Introduction
  1.1 Realistic Image Synthesis(真是图像的合成)  
  1.2 Global Illuminaton
  - Ray-Tracing Techniques
  - Finite(有限) Element Radiosity(光能传递) Techniques
  - Hybrid(杂种) and Multi-Pass Techniques
  _ Photo Mapping 
   
## Fundamentals of Globall Illumination  
2.1 The Nature of Light  
2.2 Lighting Terminology(术语)  
- Radiometry(放射线)  
- Photometry(光度法)  
- The Solid Angle  

2.3 Light Emission  
2.4 Light Scattering  
- The BSSRDF(Bidirectional Scattering Surface Reflectance Distribution Fuction)
- The BRDF(Bidirectional Reflectance Distribution Fuction)
- The Reflectance
- Diffuse Reflection
- Specular Reflection
- Reflection Modes  

2.5 The Rendering Equation
- The Radiosity Equation
- Neumann Series(诺伊曼级数) Expansion
- Path Integral Formulation  

2.6 Light Transport Notation(符号)

## Monto Carlo Ray Tracing(蒙特卡洛射线追踪)  
3.1 Classic Ray Tracing
- Algorithm  

3.2 Path Tracming  
- Algorithm  

3.3 Bidirectional Path Tracing  
- Algorithm

3.4 Metroplis(大都会) Light Transport  
- Algorithm

## The Photo-Mapping Concept  
- Motivation
- Developing the Model
- Overview

## Photo Tracing  
5.1 Photo Emission
- Diffuse Point Light
- Spherical Light
- Square Light
- Direction Light
- Complex Light
- Multiple Lights
- Projection Maps  

5.2 Photo Scattering  
- Specular Reflection
- Diffuse Reflection
- Arbitrary BRDF Reflection
- Russian Roulette

5.3 Photo Storing

## The Photo Map Data Structure  
- The Data Structure
- Photo Representation
- The Balanced Kd-Tree
- - Memory Layout
- - Balancing Algorithm

- Locating the Nearest Photons Efficiently
- - Algorithm

## The Radiance Estimate(辐射估计)  
7.1 Density Estimation  
7.2 Derivation  
7.3 Algorithm  
7.4 Filtering  
- The Cone Filter
- The Gaussian Filter
- Differential Checking

7.5 Photo Gathering(聚会)

## Visualizing the Photo Map  
8.1 Rendering Caustics(焦散)  
8.2 Rendering Color Bleeding
- Excluding Direct Illumination
  
8.3 Fast Approximations  
8.4 Caustic Examples  
- Reflection Inside a Ring
- Prism with Dispersion (棱镜分散)
- Caustics on a Non-Lambertian Surface
- A Glass of Cognac on a Rough Surface 
## A Practical Two-Pass Algorithm  
9.1 Overview  
9.2 Solving the Rendering Equation  
9.3 Pass 1: Photo Tracing
- The Caustics Photon Map
- The Global Photon Map

9.4 Pass 2:Rendering  
- Direct Illumination
- Spcular and Glossy Reflection
- Caustics
- Multiple Diffuse Reflections

## Particpationg Media  
10.1 Light Scattering in Participation  
10.2 The Volume Rendering Equation  
10.3 The Phase Function  
- Isotropic Scattering(各向同性散射)
- The Henyey-Greenstein Phase Function
- The Schlick Phase Function
- Other Phase Functions

10.4 Ray Marching  
10.5 Photo Tracing  
- Photo Scattering
- Photo Storing
- Photo Emission

10.6 The Volume Radiance Estimate  
10.7 Rendering Participating Media  
10.8 Subsurface Scattering
- Photo Tracing
- Rendering

    
