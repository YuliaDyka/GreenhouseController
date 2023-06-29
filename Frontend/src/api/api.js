import axios from "axios";

const axiosInstance = axios.create({
  badeURL: "http://localhost:5000/",
  headers: {
    "Content-Type": "application/json",
    "Access-Control-Allow-Origin": "*",
    "Access-Control-Allow-Methods": "GET, POST, PATCH, PUT, DELETE, OPTIONS",
  },
});

export const getAllPorts = async () => {
  try {
    const response = await axios.get("http://localhost:5000/ports");
    console.log("API: ", response.data);
    return response.data;
  } catch (error) {
    console.error(error);
  }
};

export const connectToPort = async (id) => {
  try {
    const response = await axios.post(
      "http://localhost:5000/ports/open",
      { index: id }
    );
    return response.data;
  } catch (error) {
    console.error(error);
  }
};
export const disconectPort = async () => {
  try {
    const response = await axios.post(
      "http://localhost:5000/ports/close"
    );
    return response.data;
  } catch (error) {
    console.error(error);
  }
};

export const getInfos = async () => {
  try {
    const response = await axios.get("http://localhost:5000/info");
    //  console.log("response: getInfos Api- " + response.data);
    return response.data;
  } catch (error) {
    console.error(error);
  }
};

export const sendDataColors = async (
  redWidth,
  greenWidth,
  blueWidth,
  brightnessWidth
) => {
  try {
    const response = await axios.post("http://localhost:5000/leds", {
      red: redWidth,
      green: greenWidth,
      blue: blueWidth,
      brightness: brightnessWidth,
    });
    return response.data;
  } catch (error) {
    console.error(error);
  }
};

export const sendPosition = async (position) => {
  try {
    const response = await axios.post("http://localhost:5000/hatch", {
      position: position,
    });
    return response.data;
  } catch (error) {
    console.error(error);
  }
};

export const sendSpeed = async (speed) => {
  try {
    const response = await axios.post("http://localhost:5000/fan", {
      speed: speed,
    });
    return response.data;
  } catch (error) {
    console.error(error);
  }
};
