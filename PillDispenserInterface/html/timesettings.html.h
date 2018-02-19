/*
 * timesettings.html.h
 *
 *  Created on: 27 Nov 2016
 *      Author: felix
 */

#ifndef TIMESETTINGS_HTML_H_
#define TIMESETTINGS_HTML_H_


#define TIMESETTINGS "<!DOCTYPE html> <html> <head> <title> Dispensing Time Settings </title> " STYLE " \
	<body> \
		<ul> \
			<li> <a href = '/' title='Connect to a Network'>Connect to a Network </a></li> \
			<li> <a href = '/notify' title='Set Notification properties'>Set Notification properties </a></li> \
			<li> <a href = '/timesettings'>Set dispensing time events</a></li> \
		</ul> \
		<p><strong>Please select when to dispense the Pills:</strong></p> \
		<template id='timeSelect' name='timeSelect'> \n \
            <div id='selectElement' name='selectElement'> \
                    <!--<fieldset class='date'>--> \
                        <input type='checkbox' name='check' id='date' value='date' /> \
                        <!--<label for='day'>Day</label>--> \
                        <select id='day' name='day' > \
                            <option>RepeatDaily</option> \
                            <option>Monday</option> \
							<option>Tuesday</option> \
                            <option>Wednesday</option> \
                            <option>Thursday</option> \
                            <option>Friday</option> \
                            <option>Saturday</option> \
                            <option>Sunday</option> \
                        </select> \n \
                        <!--<label for='hour'>Hour</label>--> \
                        <select id='hour' name='hour' > \
							<option>00</option> \
							<option>01</option> \
                            <option>02</option> \
                            <option>03</option> \
                            <option>04</option> \
                            <option>05</option> \
                            <option>06</option> \
                            <option>07</option> \
                            <option>08</option> \
                            <option>09</option> \
                            <option>10</option> \
                            <option>11</option> \
                            <option selected>12</option> \
                            <option>13</option> \
                            <option>14</option> \
                            <option>15</option> \
                            <option>16</option> \
                            <option>17</option> \
                            <option>18</option> \
                            <option>19</option> \
                            <option>20</option> \
                            <option>21</option> \
                            <option>22</option> \
                            <option>23</option> \
                        </select> \n \
                    <!--<label for='year_start'>Minute</label>--> \
                        <select id='minute' name='minute' > \
                            <option>00</option> \
                            <option>01</option> \
                            <option>02</option> \
                            <option>03</option> \
                            <option>04</option> \
                            <option>05</option> \
                            <option>06</option> \
                            <option>07</option> \
                            <option>08</option> \
                            <option>09</option> \
                            <option>10</option> \
                            <option>11</option> \
                            <option>12</option> \
                            <option>13</option> \
                            <option>14</option> \
                            <option>15</option> \
                            <option>16</option> \
                            <option>17</option> \
                            <option>18</option> \
                            <option>19</option> \
                            <option>20</option> \
                            <option>21</option> \
                            <option>22</option> \
                            <option>23</option> \
                            <option>24</option> \
                            <option>25</option> \
                            <option>26</option> \
                            <option>27</option> \
                            <option>28</option> \
                            <option>29</option> \
                            <option selected>30</option> \
                            <option>31</option> \
                            <option>32</option> \
                            <option>33</option> \
                            <option>34</option> \
                            <option>35</option> \
                            <option>36</option> \
                            <option>37</option> \
                            <option>38</option> \
                            <option>39</option> \
                            <option>40</option> \
                            <option>41</option> \
                            <option>42</option> \
                            <option>43</option> \
                            <option>44</option> \
                            <option>45</option> \
                            <option>46</option> \
                            <option>47</option> \
                            <option>48</option> \
                            <option>49</option> \
                            <option>50</option> \
                            <option>51</option> \
                            <option>52</option> \
                            <option>53</option> \
                            <option>54</option> \
                            <option>55</option> \
                            <option>56</option> \
                            <option>57</option> \
                            <option>58</option> \
                            <option>59</option> \
                        </select> \n \
            </div> \
            <span class='date'>(Day-Hour-Minute)</span> \
                    <!--</fieldset>--> \
        </template> \n \
		<form id='form'> \
        </form> \
		<form id='submitform' action='/timeselected'> \
        </form> \
        <script> \
            var date = document.getElementById('timeSelect'); \
            var form = document.getElementById('form'); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
            form.appendChild(date.content.cloneNode(true)); \
        </script> \n \
        <script> \
            function dateSubmit() { \
                var form = document.getElementById('form'); \
                var childs = form.children; \
                console.log(childs); \
                var timeSelects = []; \
                for(var i =0; i<childs.length; i+=2){ \
                    timeSelects.push(childs.item(i)); \
                } \
                console.log(timeSelects); \
                var tickedTimeSelects = []; \
                for(var i=0; i<timeSelects.length; i++){ \
                    if(timeSelects[i].firstElementChild.checked === true){ \
                        tickedTimeSelects.push(timeSelects[i]); \
                    } \
                } \
                console.log(tickedTimeSelects); \
                var submitform = document.getElementById('submitform'); \
                for(var i =0; i<tickedTimeSelects.length; i++){ \
                    console.log(tickedTimeSelects[i]); \
                    submitform.appendChild(tickedTimeSelects[i].children.item(1)); \
                    submitform.appendChild(tickedTimeSelects[i].children.item(1)); \
                    submitform.appendChild(tickedTimeSelects[i].children.item(1)); \
                } \
                console.log(submitform); \
                submitform.submit(); \
            } \n \
        </script> \
        <!--<input type='checkbox' id='check1' name='check' value='check'>--> \
        <button onclick='dateSubmit()'>Submit</button> \
    </body> \
</html> "



#endif /* TIMESETTINGS_HTML_H_ */
