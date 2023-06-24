import logo from './logo.svg';
import React, { useEffect, useState } from "react";
import './App.css';


function App() {
  return (
    <React.Fragment>
      <div className='header'>
        <h3 className="header-name">Green House manager</h3>
      </div>
      

      <div className="action-block">
        <button className="btn btn-secondary btn-sm">
          Add
        </button>
        <button className="btn btn-secondary btn-sm">
          Delete
        </button>
      </div>
      <div className="screen">

      </div>
      <div className="footer">
        Green House
      </div>
    </React.Fragment>
  );
}

export default App;
