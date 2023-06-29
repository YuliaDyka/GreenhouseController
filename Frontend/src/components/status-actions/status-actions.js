import React from "react";
import "./style.css";

export function StatusActions({
  insideTemperature,
  insideLight,
  outsideTemperature,
  outsideLight,
  lightRed,
  lightBlue,
  lightGreen,
  lightBrightness,
  hatchState,
  fanState,
  hatchPosition,
  fanSpeed,
}) {
  return (
    <>
      <div className="status-gorizont">
        <div className="status-block">
          <div className="status-action">
            <label className="input-lable" for="temperature">
              Inside temperature:
            </label>
            <input
              readOnly
              className="input-data"
              id="temperature"
              value={insideTemperature}
              type="number"
            />
          </div>
          <div className="status-action">
            <label className="input-lable" for="insideLight">
              Inside light:
            </label>
            <input
              readOnly
              className="input-data"
              id="insideLight"
              value={insideLight}
              type="number"
            />
          </div>
        </div>
        <div className="status-block">
          <div className="status-action">
            <label className="input-lable" for="outsideTemperature">
              Outside temperature:
            </label>
            <input
              readOnly
              className="input-data"
              id="outsideTemperature"
              value={outsideTemperature}
              type="number"
            />
          </div>
          <div className="status-action">
            <label className="input-lable" for="outsideLight">
              Outside light:
            </label>
            <input
              readOnly
              className="input-data"
              id="outsideLight"
              value={outsideLight}
              type="number"
            />
          </div>
        </div>
      </div>
      <div className="status-gorizont">
        <div className="status-block">
          <div className="status-action">
            <label className="input-lable" for="lightRedId">
              Light RED:
            </label>
            <input
              readOnly
              className="input-data"
              id="lightRedId"
              value={lightRed}
              type="number"
            />
          </div>
          <div className="status-action">
            <label className="input-lable" for="lightBlueId">
              Light BLUE:
            </label>
            <input
              readOnly
              className="input-data"
              id="lightBlueId"
              value={lightBlue}
              type="number"
            />
          </div>
        </div>
        <div className="status-block">
          <div className="status-action">
            <label className="input-lable" for="lightGreenId">
              Light GREEN:
            </label>
            <input
              readOnly
              className="input-data"
              id="lightGreenId"
              value={lightGreen}
              type="number"
            />
          </div>
          <div className="status-action">
            <label className="input-lable" for="lightBrightnessId">
              Light brightness:
            </label>
            <input
              readOnly
              className="input-data"
              id="lightBrightnessId"
              value={lightBrightness}
              type="number"
            />
          </div>
        </div>
      </div>
      <div className="status-gorizont">
        <div className="status-block">
          <div className="status-action">
            {/* <label className="input-lable" for="hatchStateId">
              Hatch state:
            </label>
            <input
              readOnly
              className="input-data"
              id="hatchStateId"
              value={hatchState}
              type="text"
            /> */}
          </div>
          <div className="status-action">
            {/* <label className="input-lable" for="fanStateId">
              FAN state:
            </label>
            <input
              readOnly
              className="input-data"
              id="fanStateId"
              value={fanState}
              type="text"
            /> */}
          </div>
        </div>
        <div className="status-block">
          <div className="status-action">
            <label className="input-lable" for="hatchPositionId">
              Hatch position:
            </label>
            <input
              readOnly
              className="input-data"
              id="hatchPositionId"
              value={hatchState}
              type="number"
            />
          </div>
          <div className="status-action">
            <label className="input-lable" for="fanSpeedId">
              FAN speed:
            </label>
            <input
              readOnly
              className="input-data"
              id="fanSpeedId"
              value={fanSpeed}
              type="number"
            />
          </div>
        </div>
      </div>
    </>
  );
}
