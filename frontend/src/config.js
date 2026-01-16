// frontend/src/config.js

const environment = process.env.REACT_APP_ENVIRONMENT || 'dev';
console.log(`Loading configuration for environment: ${environment}`);
// Import the correct configuration file based on environment
let sharedConfig;
try {
    if (environment === 'prod') {
        sharedConfig = require('./prod.json'); // Production config
        sharedConfig.API_KEY = process.env.REACT_APP_API_KEY;
    } else {
        sharedConfig = require('./dev.json');// Development config
    }
} catch (error) {
    console.error(`Failed to load configuration for environment: ${environment}`, error);
    sharedConfig = {}; // Fallback to an empty config if there's an error
}

// Construct additional computed values based on loaded config
sharedConfig.API_BASE_URL = `http://127.0.0.1:5003`;
// sharedConfig.API_BASE_URL = `https://iot-project-6i3k.onrender.com`;
// Export the entire configuration object
export default sharedConfig;
