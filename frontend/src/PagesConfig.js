import {Home} from '@mui/icons-material';
import GroupsIcon from '@mui/icons-material/Groups';
import SettingsIcon from '@mui/icons-material/Settings';
import QueryStatsIcon from '@mui/icons-material/QueryStats';


import WelcomePage from './components/WelcomePage';
import DashboardPage from "./components/DashboardPage";
import SettingsPage from "./components/SettingsPage";
import ClassroomPage from "./components/ClassroomPage";
export const appPages = [
    {
        name: "Welcome",
        path: "/",
        component: <WelcomePage/>,
        icon: () => <Home/>,
    }
    , {
        name: "Classroom",
        path: "/Classroom",
        component: <ClassroomPage/>,
        icon: () => <GroupsIcon/>,
    },
    {
        name: "Stats",
        path: "/stats",
        component: <DashboardPage/>,
        icon: () => <QueryStatsIcon/>,
    },
    {
        name: "Settings",
        path: "/settings",
        component: <SettingsPage/>,
        icon: () => <SettingsIcon/>,
    }
];