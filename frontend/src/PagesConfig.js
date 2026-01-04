import { Home, AccountCircle } from '@mui/icons-material';
import SportsSoccerIcon from '@mui/icons-material/SportsSoccer';
import SportsIcon from '@mui/icons-material/Sports';
import GroupsIcon from '@mui/icons-material/Groups';
import CalendarTodayIcon from '@mui/icons-material/CalendarToday';


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
        icon: () => <SportsIcon />,
    },
    {
        name: "Settings",
        path: "/settings",
        component: <SettingsPage/>,
        icon: () => <SportsIcon />,
    }
];