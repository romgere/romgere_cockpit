module.exports = {
  title: 'Romgere Cockpit Arduino Lib',
  description: 'Arduino library for building \"real\" cockpit/panel with X-Plane and Arduino board',
  themeConfig: {
    logo: '/logo.png',
    repo: 'romgere/romgere_cockpit',
    repoLabel: 'Github',
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Example', link: '/2-basic-example.html' }
    ],
    sidebarDepth: 3,
    activeHeaderLinks: false, // Default: true
    displayAllHeaders: true, // Default: false
    sidebar: [
      ['/', 'Home'],
      ['/3-command-and-data.html', 'X-Plane command'],
      {
        title: 'Library Controls',
        path: '/4-controls.html',
        collapsable: false,
        children: [
          ['controls/ctrl-2pos-toggle.md', 'ArduinoToggleSwitchControl'],
          ['controls/ctrl-3pos-toggle.md', 'ArduinoThreePosToggleSwitchControl'],
          ['controls/ctrl-incremental-3pos-toggle.md', 'ArduinoIncrementalThreePosToggleSwitchControl'],
          ['controls/ctrl-push-button.md', 'ArduinoPushButtonControl'],
          ['controls/ctrl-rotary-encoder.md', 'ArduinoRotaryEncoderControl'],
          ['controls/ctrl-rotary-switch.md', 'ArduinoRotarySwitchControl'],
          ['controls/ctrl-potentiometer.md', 'ArduinoPotentiometerControl']
        ]
      },

      ['/1-configuration-reference.html', 'Configuration reference'],
    ]
  },
  plugins: [
    '@vuepress/medium-zoom',
    '@vuepress/back-to-top',
    [
      'sitemap',
      {
        hostname: 'https://romgere-cockpit-arduino-lib.mestres.fr'
      }
    ],
    [
      "vuepress-plugin-matomo",
      {
        'siteId': 3,
        'trackerUrl': "https://stats.madgency.fr/"
      },
    ]
  ]
}
