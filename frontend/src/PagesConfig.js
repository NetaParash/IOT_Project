import { Home, AccountCircle } from '@mui/icons-material';
import SportsSoccerIcon from '@mui/icons-material/SportsSoccer';
import SportsIcon from '@mui/icons-material/Sports';
import GroupsIcon from '@mui/icons-material/Groups';
import CalendarTodayIcon from '@mui/icons-material/CalendarToday';


import WelcomePage from './components/WelcomePage';
import IoTPage from "./components/IoTPage";
import DashboardPage from "./components/DashboardPage";
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
        icon: () => <SportsIcon />,
    },
    {
        name: "IoT Project",
        path: "/iot_project",
        component: <IoTPage/>,
        icon: () => <AccountCircle />,
    },
];