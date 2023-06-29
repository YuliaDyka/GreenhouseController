import React from "react";
import { useState, useEffect } from "react";
import "./App.css";
import Grid from "@mui/material/Grid";

import { StatusActions } from "./components/status-actions/status-actions";
import {
  getAllPorts,
  connectToPort,
  disconectPort,
  sendDataColors,
  sendPosition,
  sendSpeed,
  getInfos,
} from "./api/api";
import MenuItem from "@mui/material/MenuItem";
import FormControl from "@mui/material/FormControl";

import Select from "@mui/material/Select";

function App() {
  const [ports, setPorts] = useState([]);
  const [isConnectedPort, setConnectedPort] = useState(false);

  useEffect(() => {
    getPorts();
  }, []);

  useEffect(() => {
    setInterval(() => {
      getInfoDatas();
    }, 2000);
  }, []);

  const [portId, setPortId] = useState(0);
  const onChangePortItem = (event) => {
    setPortId(event.target.value);
    console.log("Port -- onChange: " + portId);
  };

  const clickConectDisconect = () => {
    if (isConnectedPort) {
      disconectPort()
        .then((result) => {
          console.log("response: disconectPort - " + result.result);
          getPorts();
        })
        .catch((error) => {
          console.log("no server response ", error);
          setPorts([]);
        });
    } else {
      connectToPort(portId)
        .then((result) => {
          console.log("response: conectPort -" + result.result);
          getPorts();
        })
        .catch((error) => {
          console.log("no server response ", error);
          setPorts([]);
        });
    }
  };

  const [status, setStatus] = useState("'Disconnected'");
  const [insideTemperature, setInsideTemperature] = useState(0);
  const [insideLight, setInsideLight] = useState(0);
  const [outsideTemperature, setOutsideTemperature] = useState(0);
  const [outsideLight, setOutsideLight] = useState(0);
  const [lightRed, setLightRed] = useState(0);
  const [lightBlue, setLightBlue] = useState(0);
  const [lightGreen, setLightGreen] = useState(0);
  const [lightBrightness, setLightBrightness] = useState(0);
  const [hatchState, setHatchState] = useState("---");
  const [fanState, setFanState] = useState("---");
  const [hatchPosition, setHatchPosition] = useState(0);
  const [fanSpeed, setFanSpeed] = useState(0);

  const [redWidth, setRedWidth] = useState(0);
  const changeRedWidth = (event) => {
    setRedWidth(event.target.value);
  };
  const [greenWidth, setGreenWidth] = useState(0);
  const changeGreenWidth = (event) => {
    setGreenWidth(event.target.value);
  };
  const [blueWidth, setBlueWidth] = useState(0);
  const changeBlueWidth = (event) => {
    setBlueWidth(event.target.value);
  };
  const [brightnessWidth, setBrightnessWidth] = useState(0);
  const changeBrightnessWidth = (event) => {
    setBrightnessWidth(event.target.value);
  };

  const getInfoDatas = () => {
    getInfos()
      .then((result) => {
        setInsideTemperature(result.insideTemp);
        setInsideLight(result.insideLight);
        setOutsideTemperature(result.outsideTemp);
        setOutsideLight(result.outsideLight);
        setLightRed(result.ledRed);
        setLightBlue(result.ledBlue);
        setLightGreen(result.ledGreen);
        setLightBrightness(result.ledBrightness);
        setHatchState(result.hatchPosition);
        //setFanState(result.);
        // setHatchPosition(result.position);
        setFanSpeed(result.fanSpeed * 15);
      })
      .catch((error) => {
        console.log("no server response - getInfos ", error);
        setPorts([]);
      });
  };

  const applyActionColors = () => {
    sendDataColors(redWidth, greenWidth, blueWidth, brightnessWidth)
      .then((result) => {
        console.log("response: sendDataColors - " + result.result);
      })
      .catch((error) => {
        console.log("no server response sendDataColors", error);
      });
  };

  const [position, setPosition] = useState(0);
  const changePosition = (event) => {
    setPosition(event.target.value);
  };
  const applyPosition = () => {
    sendPosition(position)
      .then((result) => {
        console.log("response: applyPosition - " + result.result);
      })
      .catch((error) => {
        console.log("no server response applyPosition ", error);
      });
  };
  const [speepNumber, setSpeepNumber] = useState(0);
  const changeSpeepNumber = (event) => {
    setSpeepNumber(event.target.value);
  };
  const applySpeepNumber = () => {
    sendSpeed(speepNumber / 15)
      .then((result) => {
        console.log("response: applySpeepNumber - " + result.result);
      })
      .catch((error) => {
        console.log("no server response applySpeepNumber ", error);
      });
  };

  return (
    <React.Fragment>
      <div className="header">
        <h3 className="header-name">Green House manager</h3>
      </div>
      <div className="action-popup">
        <div className="action-block">
          <h5>Conection</h5>
          <hr></hr>
          <div className="port-blocK">
            <Grid sx={{ display: "flex", justifyContent: "space-between" }}>
              <Grid item>
                <FormControl fullWidth>
                  <label id="label_id">Select port...</label>
                  <Select
                    disabled={isConnectedPort}
                    label="label_id"
                    value={portId}
                    onChange={onChangePortItem}
                  >
                    {createMenuItems()}
                  </Select>
                </FormControl>
              </Grid>
              <Grid item sx={{ ml: "50px", my: "auto" }}>
                <Grid container spacing={2}>
                  <Grid item>
                    <button
                      className="btn btn-secondary btn-sm"
                      onClick={clickConectDisconect}
                    >
                      {isConnectedPort ? "Disconect" : "Connect"}
                    </button>
                  </Grid>
                </Grid>
              </Grid>
            </Grid>
          </div>
          <p className="port-block-status">
            Status:<b> {status} </b>
          </p>
        </div>
        <div className="action-block">
          <h5>Status</h5>
          <hr></hr>
          <StatusActions
            insideTemperature={insideTemperature}
            insideLight={insideLight}
            outsideTemperature={outsideTemperature}
            outsideLight={outsideLight}
            lightRed={lightRed}
            lightBlue={lightBlue}
            lightGreen={lightGreen}
            lightBrightness={lightBrightness}
            hatchState={hatchState}
            fanState={fanState}
            hatchPosition={hatchPosition}
            fanSpeed={fanSpeed}
          />
        </div>

        <div className="action-block">
          <h5>Light control</h5>
          <hr></hr>

          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="redWidthId">
                Red: <span className="lable-width"></span>
              </label>
              <input
                className="input-value"
                readOnly
                type="number"
                value={redWidth}
              ></input>
            </div>
            <div className="color-block">
              <input
                type="range"
                onChange={changeRedWidth}
                min={0}
                max={255}
                step={1}
                value={redWidth}
                id="redWidthId"
              ></input>
            </div>
          </div>
          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="greenWidthId">
                Green:
              </label>
              <input
                className="input-value"
                readOnly
                type="number"
                value={greenWidth}
              ></input>
            </div>
            <div className="color-block">
              <input
                type="range"
                onChange={changeGreenWidth}
                min={0}
                max={255}
                step={1}
                value={greenWidth}
                id="greenWidthId"
              ></input>
            </div>
          </div>
          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="blueWidthId">
                Blue:
                {/* <span className="lable-width">{blueWidth}</span> */}
              </label>
              <input
                className="input-value"
                readOnly
                type="number"
                value={blueWidth}
              ></input>
            </div>
            <div className="color-block">
              <input
                type="range"
                onChange={changeBlueWidth}
                min={0}
                max={255}
                step={1}
                value={blueWidth}
                id="blueWidthId"
              ></input>
            </div>
          </div>
          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="brightnessWidthId">
                Brightness:
                <input
                  className="input-value"
                  readOnly
                  type="number"
                  value={brightnessWidth}
                ></input>
              </label>
            </div>
            <div className="brightness-block-info">
              <span>%</span>
              <input
                type="range"
                onChange={changeBrightnessWidth}
                min={0}
                max={100}
                step={1}
                value={brightnessWidth}
                id="brightnessWidthId"
              ></input>
            </div>
          </div>
          <div className="colors-aplay">
            <button
              className="btn btn-secondary btn-sm btn-colors-aplay"
              onClick={applyActionColors}
            >
              Apply
            </button>
          </div>
        </div>
        <div className="action-block">
          <h5>Hatch control</h5>
          <hr></hr>

          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="positionId">
                Position:
              </label>
              <input
                className="input-value"
                readOnly
                type="number"
                value={position}
              ></input>
              <span>%</span>
            </div>
            <div className="color-block">
              <input
                type="range"
                onChange={changePosition}
                min={0}
                max={100}
                step={1}
                value={position}
                id="positionId"
              ></input>
            </div>
          </div>
          <div className="colors-aplay">
            <button
              className="btn btn-secondary btn-sm btn-colors-aplay"
              onClick={applyPosition}
            >
              Apply
            </button>
          </div>
        </div>
        <div className="action-block">
          <h5>Fan control</h5>
          <hr></hr>

          <div className="gorizont">
            <div className="color-block">
              <label className="slider-lable" for="speepNumberId">
                Speed:
              </label>
              <input
                className="input-value"
                readOnly
                type="number"
                value={speepNumber}
              ></input>
              <span>rpm</span>
            </div>
            <div className="color-block">
              <input
                type="range"
                onChange={changeSpeepNumber}
                min={0}
                max={4500}
                step={1}
                value={speepNumber}
                id="speepNumberId"
              ></input>
            </div>
          </div>
          <div className="colors-aplay">
            <button
              className="btn btn-secondary btn-sm btn-colors-aplay"
              onClick={applySpeepNumber}
            >
              Apply
            </button>
          </div>
        </div>
      </div>
    </React.Fragment>
  );

  function createMenuItems() {
    let items = [];
    for (let i = 0; i < ports.length; i++) {
      items.push(
        <MenuItem key={i} value={i}>
          {ports[i]}
        </MenuItem>
      );
    }
    return items;
  }

  function getPorts() {
    getAllPorts()
      .then((result) => {
        setPorts(result.ports);
        console.log(result.ports, result.isOpen);

        setConnectedPort(result.isOpen);
        setStatus(result.isOpen ? "'Connected'" : "'Disconnected'");
      })
      .catch((error) => {
        console.log("no server response ", error);
        setPorts([]);
      });
  }
}

export default App;
