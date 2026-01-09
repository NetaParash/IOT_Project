import { Home, AccountCircle } from '@mui/icons-material';
import DashboardIcon from '@mui/icons-material/Dashboard';
import SportsIcon from '@mui/icons-material/Sports';
import SettingsIcon from '@mui/icons-material/Settings';import CalendarTodayIcon from '@mui/icons-material/CalendarToday';


import WelcomePage from './components/WelcomePage';
import DashboardPage from "./components/DashboardPage";
import SettingsPage from "./components/SettingsPage";
export const appPages = [
    {
        name: "Welcome",
        path: "/",
        component: <WelcomePage/>,
        icon: () => <Home />,
    },
    {
        name: "Dashboard",
        path: "/dashboard",
        component: <DashboardPage/>,
        icon: () => <DashboardIcon />,
    },
    {
        name: "Settings",
        path: "/settings",
        component: <SettingsPage/>,
        icon: () => <SettingsIcon />,
    }
];