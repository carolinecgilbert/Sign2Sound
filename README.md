# ECE 47700 Team 10 - Sign2Sound
Sign2Sound is a semi-portable ASL translator for the 24 stationary letters of the ASL alphabet. The device consists of a microcontroller, host computer, camera, character display, touch screen display, speaker, and right-handed color glove. The device streams feedback from the camera to the embedded touch screen, allowing the user to frame their gloved hand properly for the camera to capture it. The glove makes ASL sign recognition less biased by standardizing differences in the users’ hand shape, size, and color. Once the user is ready, they can press a button to trigger an image capture. Next, the image is processed on the host computer that houses the team-designed computer vision algorithm, associating signs with their English alphabet translation. Then, the corresponding translation is sent to the microcontroller via UART. The microcontroller then outputs the translated letter data both on the character display and speaker. For the audio output, the microcontroller transmits the identified letter’s audio file to a DAC module via I2C. Subsequently, the converted analog signal is output from the DAC to a speaker using a team-designed amplifier circuit. As for the visual output, the microcontroller transmits the identified letter to the character display via SPI. The device will be powered by a wall wart transformer, supplying 5V directly to the host computer. Power will be fed from the host computer to an LDO to reduce the voltage to 3.3V for the microcontroller.

## Team Members
- Team Leader: Jenna Gillette
- Software Engineer: Caroline Gilbert
- Systems Engineer: Becca Horwatt
- Hardware Engineer: Kaushik Karthikeyan

## Computer Vision Algorithm
### PCA Jupyter Notebook Instructions
Use a virtual environment to install required dependencies for the jupyter notebook.

See [Virtual Environment Dependencies](pca-test/environment.yml)

Navigate to the pca-test folder and run the following commands to activate a virtual environment on Anaconda to install and run the notebook:
- Create a new virtual environment `myenv` from the [environment.yml](pca-test/environment.yml) dependencies:

    ```
    conda create --name myenv --file environment.yml
    ```

- Activate the environment:
    ```
    conda activate myenv
    ```

- After finishing work in environment, deactivate it
    ```
    conda deactivate
    ```
- Delete environment if needed
    ```
    conda remove -n myenv --all
    ```
