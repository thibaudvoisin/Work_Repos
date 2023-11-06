import { useEffect, useState } from 'react';

const useGeoLocation = () => {
  const [latitude, setLatitude] = useState(null);
  const [longitude, setLongitude] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    if (!navigator.geolocation) {
      setError('Geolocation is not supported by your browser');
      setLoading(false);
      return;
    }

    const successCallback = (position) => {
      setLatitude(position.coords.latitude);
      setLongitude(position.coords.longitude);
      setLoading(false);
    };

    const errorCallback = (error) => {
      setError('Error retrieving your location: ' + error.message);
      setLoading(false);
    };

    navigator.geolocation.getCurrentPosition(successCallback, errorCallback);
  }, []);

  return { latitude, longitude, loading, error };
};

export default useGeoLocation;
